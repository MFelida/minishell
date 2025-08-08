/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 18:07:47 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "fake_parser.h"
#include "libft.h"
#include "redirect.h"
#include "redirect_types.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>

	// ft_fprintf(STDERR_FILENO, "%s: %s: %s\n",
	// 	__FILE_NAME__, func, strerror(errno));
void	_clean_before_exit(t_cmd_params params)
{
	close_fds(params.open_fds);
	free_cmd_params(params);
}

_Noreturn void	cmd_exec(t_cmd_params params)
{
	int	find_bin_ret;

	find_bin_ret = find_bin(params.bin_path, params.cmd_args[0]);
	if (find_bin_ret > 0)
	{
		ft_fprintf(STDERR_FILENO,
			"%s: command not found\n",
			params.cmd_args[0]);
		_clean_before_exit(params);
		exit(MS_CMD_NOT_FOUND);
	}
	if (find_bin_ret < 0)
	{
		ft_fprintf(STDERR_FILENO, "%s: permission denied", params.cmd_args[0]);
		_clean_before_exit(params);
		exit(MS_PERM_DENIED);
	}
	if (do_redirs(&params))
	{
		_clean_before_exit(params);
		exit(MS_FAILURE);
	}
	close_fds(params.open_fds);
	execve(params.bin_path, params.cmd_args, params.envp);
	ft_fprintf(STDERR_FILENO, "%s: %s: %s\n",
		__FILE_NAME__, "execve", strerror(errno));
	free_cmd_params(params);
	exit(MS_FAILURE);
}

int	cmd_run(t_cmd_params params, t_parse_node *node)
{
	t_cmd_params	*params_node;

	params.cmd_args = make_argv(node);
	for (int i = 0; params.cmd_args[i]; i++) {
		printf("%s%s", i != 0 ? ";" : "", params.cmd_args[i]);
	}
	printf("\n");
	if (!params.cmd_args)
		return (1);
	errno = 0;
	params.pid = fork();
	if (params.pid < 0)
	{
		ft_fprintf(STDERR_FILENO,
			"%s: %s: %s\n", __FILE_NAME__, "fork", strerror(errno));
		return (1);
	}
	if (params.pid == 0)
		cmd_exec(params);
	params_node = malloc(sizeof(t_cmd_params));
	if (!params_node)
		return (1);
	*params_node = params;
	ft_lstadd_back((t_list **) params.head, (t_list *) params_node);
	return (0);
}

int	cmd_pipe(t_cmd_params params, t_parse_node	*node)
{
	t_cmd_params	writer;
	t_cmd_params	reader;
	t_pipe			p;
	int				retval;

	if (pipe(p.a) < 0)
		return (MS_CMD_ERROR_PIPE);
	ft_lstadd_back((t_list **) params.open_fds, (t_list *) new_fd(p.read));
	ft_lstadd_back((t_list **) params.open_fds, (t_list *) new_fd(p.write));
	writer = params;
	if (add_redir(&writer,
			(t_redir_src){.type = MS_REDIR_FD, .fd = p.write}, 
			(t_redir_dest){.type = MS_REDIR_FD, .fd = STDOUT_FILENO}))
		return (MS_CMD_ERROR_PIPE);
	reader = params;
	if (add_redir(&reader,
			(t_redir_src){.type = MS_REDIR_FD, .fd = p.read}, 
			(t_redir_dest){.type = MS_REDIR_FD, .fd = STDIN_FILENO}))
		return (MS_CMD_ERROR_PIPE);
	retval = cmd_next_node(&writer, node->children[0])
		|| cmd_next_node(&reader, node->children[1]);
	return (retval);
}

int	cmd_input_redir(t_cmd_params params, t_parse_node *node)
{
	int	retval;

	retval = add_redir(&params,
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[0]->tok.id.value, .mode = O_RDONLY},
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDIN_FILENO});
	if (retval)
		return (retval);
	return (cmd_next_node(&params, node->children[1]));
}

int	cmd_output_redir(t_cmd_params params, t_parse_node *node)
{
	int	retval;

	retval = add_redir(&params,
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[1]->tok.id.value, .mode = O_WRONLY | O_CREAT, .flags = 0644});
	if (retval)
		return (retval);
	return (cmd_next_node(&params, node->children[0]));
}

int	cmd_output_append(t_cmd_params params, t_parse_node *node)
{
	int	retval;

	retval = add_redir(&params,
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[1]->tok.id.value, .mode = O_APPEND});
	if (retval)
		return (retval);
	return (cmd_next_node(&params, node->children[0]));
}

int	cmd_next_node(t_cmd_params *params, t_parse_node *node)
{
	t_fp_ops	op;

	op = node->tok.op.op;
	if (op == FP_OP_CMD)
		return (cmd_run(*params, node));
	if (op == FP_OP_PIPE)
		return (cmd_pipe(*params, node));
	if (op == FP_OP_FILE_INPUT)
		return (cmd_input_redir(*params, node));
	if (op == FP_OP_FILE_OUTPUT)
		return (cmd_output_redir(*params, node));
	if (op == FP_OP_FILE_APPEND)
		return (cmd_output_append(*params, node));
	return (1);
}
