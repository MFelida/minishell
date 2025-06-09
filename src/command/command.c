/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/02 17:53:34 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "fake_parser.h"
#include "libft.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <stddef.h>
#include <unistd.h>

int	find_bin(char *dest, const char *name);

t_cmd_params	cmd_params_default(void)
{
	t_cmd_params	res;
	extern char		**environ;

	res.stdin = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = STDIN_FILENO};
	res.stdout = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = STDOUT_FILENO};
	res.stderr = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = STDERR_FILENO};
	res.pid = -1;
	res.envp = environ;
	res.wstatus = -1;
	res.rusage = (struct rusage){0};
	res.cmd_args = NULL;
	res.bin_path[0] = '\0';
	return (res);
}

char	**make_argv(t_parse_node *node)
{
	char	**res;
	int		argc;
	int		i;

	argc = count_chld_nodes(node);
	res = ft_calloc((argc + 1), sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (node->children[i])
	{
		res[i] = node->children[i]->tok.id.value;
		i++;
	}
	return (res);
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
		// TODO free stuff
		exit(MS_CMD_NOT_FOUND);
	}
	if (find_bin_ret < 0)
	{
		ft_fprintf(STDERR_FILENO, "%s: permission denied", params.cmd_args[0]);
		// TODO free stuff
		exit(MS_PERM_DENIED);
	}
	// TODO do_redirs
	execve(params.bin_path, params.cmd_args, params.envp);
	ft_fprintf(STDERR_FILENO, "%s: %s: %s\n",
		__FILE_NAME__, "execve", strerror(errno));
	exit(EXIT_FAILURE);
}

int	cmd_run(t_cmd_params params, t_parse_node *node)
{
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
	// TODO: Push to child list
	return (0);
}

int	cmd_pipe(t_cmd_params params, t_parse_node	*node)
{
	t_cmd_params	writer;
	t_cmd_params	reader;
	int				pipefd[2];
	int				retval;

	if (pipe(pipefd))
		return (1);
	writer = params;
	writer.stdout.fd = pipefd[1];
	writer.stdout.type = MS_CMD_IO_FD;
	reader = params;
	reader.stdin.fd = pipefd[0];
	reader.stdin.type = MS_CMD_IO_FD;
	retval = cmd_next_node(writer, node->children[0])
		|| cmd_next_node(reader, node->children[1]);
	close(pipefd[0]);
	close(pipefd[1]);
	return (retval);
}

int	cmd_next_node(t_cmd_params params, t_parse_node *node)
{
	if (node->tok.op.op == FP_OP_CMD)
		return (cmd_run(params, node));
	if (node->tok.op.op == FP_OP_PIPE)
		return (cmd_pipe(params, node));
	return (1);
}
