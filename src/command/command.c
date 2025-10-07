/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 23:33:37 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "parse_tree.h"
#include "libft.h"
#include "parser.h"
#include "redirect.h"
#include "redirect_types.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <stddef.h>
#include <sys/wait.h>
#include <unistd.h>

static void	_clean_before_exit(t_cmd_params params)
{
	free_cmd_params(params);
	del_redir_list(&params.redirs);
}

_Noreturn void	cmd_exec(t_cmd_params params)
{
	int			find_bin_ret;
	struct stat	sb;

	find_bin_ret = find_bin(params.bin_path, params.cmd_args[0]);
	if (find_bin_ret)
	{
		if (find_bin_ret == MS_CMD_NOT_FOUND)
			ft_print_err("command not found", 2, "minishell", params.cmd_args[0]);
		else if (find_bin_ret == MS_PERM_DENIED)
			ft_print_err("Permission denied", 2, "minishell", params.cmd_args[0]);
		else
			ft_print_err(strerror(errno), 2, "minishell", params.cmd_args[0]);
		_clean_before_exit(params);
		ft_exit(find_bin_ret);
	}
	if (stat(params.bin_path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_print_err("Is a directory", 2, "minishell", params.cmd_args[0]);
		_clean_before_exit(params);
		ft_exit(MS_PERM_DENIED);
	}
	if (do_redirs(&params))
	{
		_clean_before_exit(params);
		ft_exit(MS_FAILURE);
	}
	close_fds();
	params.envp	 = ms_getenv_full(0, 1, 1);
	execve(params.bin_path, params.cmd_args, params.envp);
	free_parse_tree(&params.pt);
	ft_print_err(strerror(errno), 2, "minishell", params.cmd_args[0]);
	_clean_before_exit(params);
	ft_exit(MS_PERM_DENIED);
}

int	bltin_run(t_cmd_params params, t_parse_node *node)
{
	t_cmd_params	*params_node;
	int				res;

	params.context |= MS_CMD_CONTEXT_BLTIN;
	params.cmd_args = make_argv(node);
	if (!params.cmd_args)
		return (MS_CMD_ERROR_MALLOC);
	res = do_builtin(params.cmd_args[0], &params);
	params_node = malloc(sizeof(t_cmd_params));
	if (!params_node)
		res |= (MS_CMD_ERROR_MALLOC);
	else
	{
		*params_node = params;
		ft_lstadd_back((t_list **) params.head, (t_list *) params_node);
	}
	return (res);
}

int	cmd_run(t_cmd_params params, t_parse_node *node)
{
	t_cmd_params	*params_node;

	params.cmd_args = make_argv(node);
	if (!params.cmd_args)
		return (1);
	params.context |= MS_CMD_CONTEXT_SIMPLE;
	errno = 0;
	params.pid = fork();
	if (params.pid < 0)
	{
		ft_print_err(strerror(errno), 2, __FILE_NAME__, "fork");
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
	t_list			*last;

	if (ms_pipe(&p) < 0)
		return (MS_CMD_ERROR_PIPE);
	params.context |= MS_CMD_CONTEXT_PIPE;
	writer = params;
	reader = params;
	last = ft_lstlast((t_list *) params.redirs);
	if (add_redir(&writer,
			(t_redir_src){.type = MS_REDIR_FD, .fd = p.write}, 
			(t_redir_dest){.type = MS_REDIR_FD, .fd = STDOUT_FILENO}))
		return (MS_CMD_ERROR_PIPE);
	retval = cmd_next_node(&writer, node->children[0]);
	if (last)
		ft_lstclear((t_list **) &last->next, free);
	if (add_redir(&reader,
			(t_redir_src){.type = MS_REDIR_FD, .fd = p.read}, 
			(t_redir_dest){.type = MS_REDIR_FD, .fd = STDIN_FILENO}))
		return (MS_CMD_ERROR_PIPE);
	retval |= cmd_next_node(&reader, node->children[1]);
	if (last)
		ft_lstclear((t_list **) &last->next, free);
	else
	{
		ft_lstclear((t_list **) &writer.redirs, free);
		ft_lstclear((t_list **) &reader.redirs, free);
	}
	return (retval);
}

int	cmd_hear_doc(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
					(t_redir_dest){.type = MS_REDIR_FD, .fd = node->tok.op.value},
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDIN_FILENO});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	if (last)
		ft_lstclear((t_list **) &last->next, free);
	return (retval);
}

int	cmd_input_redir(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[0]->tok.id.value, .mode = O_RDONLY},
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDIN_FILENO});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}

int	cmd_output_redir(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[0]->tok.id.value, .mode = O_WRONLY | O_CREAT | O_TRUNC, .flags = 0644});
	if (retval)
		return (retval);
	retval =  cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}

int	cmd_output_append(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
					(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
					(t_redir_dest){.type = MS_REDIR_FILE, .file = node->children[0]->tok.id.value, .mode = O_APPEND | O_CREAT | O_WRONLY, .flags = 0644});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}

int	cmd_next_node(t_cmd_params *params, t_parse_node *node)
{
	t_ms_token_type	type;
	t_ms_ops	op;

	type = node->tok.type;
	if (type == MS_TOK_ERROR || type == MS_TOK_IDENTIFIER)
		return (MS_CMD_ERROR_FAILURE);
	if (type != MS_TOK_OP)
		return (MS_CMD_ERROR_FAILURE);
	op = node->tok.op.op;
	if (op == MS_OP_ERROR)
		return (MS_CMD_ERROR_FAILURE);
	if (op == MS_OP_BLTIN)
		return (bltin_run(*params, node));
	if (op == MS_OP_CMD)
		return (cmd_run(*params, node));
	if (op == MS_OP_FILE_INPUT)
		return (cmd_input_redir(*params, node));
	if (op == MS_OP_HEREDOC)
		return (cmd_hear_doc(*params, node));
	if (op == MS_OP_FILE_OUTPUT)
		return (cmd_output_redir(*params, node));
	if (op == MS_OP_FILE_APPEND)
		return (cmd_output_append(*params, node));
	if (op == MS_OP_PIPE)
		return (cmd_pipe(*params, node));
	return (MS_CMD_ERROR_FAILURE);
}
