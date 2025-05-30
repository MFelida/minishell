/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/30 16:57:32 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <stddef.h>
#include <unistd.h>
#include "fake_parser.h"
#include "libft.h"

char	*find_bin(const char* name);

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
	return (res);
}

char **make_argv(t_parse_node **nodes)
{
	char	**res;
	size_t	n_args;
	size_t	i;

	n_args = 0;
	while(nodes[n_args])
		n_args++;
	res = malloc((n_args + 1) * sizeof(char*));
	if (!res)
		return (NULL);
	i = 0;
	while (nodes[i])
	{
		res[i] = nodes[i]->tok.id.value;
		i++;
	}
	res[i] = NULL;
	return (res);
}

int	io_to_fd(t_cmd_io io, int fd)
{
	errno = 0;
	if (io.type == MS_CMD_IO_FILE)
	{
		io.fd = open(io.file.name, io.file.flags, 0644);
		if (io.fd < 0)
		{
			ft_fprintf(STDERR_FILENO, "%s: %s: %s\n", __FILE_NAME__, "open", strerror(errno));
			return (1);
		}
		io.type = MS_CMD_IO_FD;
	}
	if (io.type == MS_CMD_IO_FD && io.fd != fd)
	{
		if (dup2(io.fd, fd))
			ft_fprintf(STDERR_FILENO, "%s: %s: %s\n", __FILE_NAME__, "dup2", strerror(errno));
		if (close(io.fd))
			ft_fprintf(STDERR_FILENO, "%s: %s: %s\n", __FILE_NAME__, "close", strerror(errno));
	}
	if (io.type == MS_CMD_IO_ERROR)
	{
		ft_fprintf(2, "%s: io_to_fd: io type unknown\n");
		return (1);
	}
	return (!!errno);
}

_Noreturn void	cmd_exec(t_cmd_params params)
{
	char	*bin_path;

	bin_path = find_bin(params.cmd_args[0]);
	io_to_fd(params.stdin, STDIN_FILENO);
	io_to_fd(params.stdout, STDOUT_FILENO);
	io_to_fd(params.stderr, STDERR_FILENO);
	execve(bin_path, params.cmd_args, params.envp);
	ft_fprintf(STDERR_FILENO, "%s: %s: %s\n", __FILE_NAME__, "execve", strerror(errno));
	exit(EXIT_FAILURE);
}

int	cmd_run(t_cmd_params params, t_parse_node *node)
{
	params.cmd_args = make_argv(node->children);
	if (!params.cmd_args)
		return (1);
	errno = 0;
	params.pid = fork();
	if (params.pid < 0)
	{
		ft_fprintf(STDERR_FILENO, "%s: %s: %s\n", __FILE_NAME__, "fork", strerror(errno));
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

	if (pipe(pipefd))
		return (1);
	writer = params;
	writer.stdout.fd = pipefd[1];
	writer.stdout.type = MS_CMD_IO_FD;
	reader = params;
	reader.stdin.fd = pipefd[0];
	reader.stdin.type = MS_CMD_IO_FD;
	if (cmd_next_node(writer, node->children[0])
		|| cmd_next_node(reader, node->children[1]))
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

int	cmd_next_node(t_cmd_params params, t_parse_node *node)
{
	if (node->tok.op.op == FP_OP_CMD)
		return (cmd_run(params, node));
	if (node->tok.op.op == FP_OP_PIPE)
		return (cmd_pipe(params, node));
	return (1);
}
