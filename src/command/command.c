/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/30 12:10:06 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"

#include <stdlib.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <stddef.h>
#include <unistd.h>
#include "fake_parser.h"
#include "libft.h"

t_cmd_params	cmd_params_default(void)
{
	t_cmd_params	res;
	extern char		**environ;

	res.stdin = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = 0};
	res.stdout = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = 1};
	res.stderr = (t_cmd_io){.type = MS_CMD_IO_FD, .fd = 2};
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

int	cmd_run(t_cmd_params params, t_parse_node *node)
{
	params.cmd_args = make_argv(node->children);
	if (!params.cmd_args)
		return (1);
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
