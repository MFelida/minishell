/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 14:18:45 by mifelida         ###   ########.fr       */
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
	params_node = malloc(sizeof(t_cmd_params));
	if (!params_node)
		return (MS_CMD_ERROR_MALLOC);
	*params_node = params;
	ft_lstadd_back((t_list **) params.head, (t_list *) params_node);
	errno = 0;
	params.pid = fork();
	if (params.pid < 0)
	{
		ft_print_err(strerror(errno), 2, __FILE_NAME__, "fork");
		return (MS_CMD_ERROR_FAILURE);
	}
	if (params.pid == 0)
		cmd_exec(params);
	return (0);
}

static int	_handle_pipe_end(int *retval,
							t_cmd_params params,
							t_redir redir,
							t_parse_node *node)
{
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	if (add_redir(&params, redir.src, redir.dest))
		return (MS_CMD_ERROR_PIPE);
	*retval |= cmd_next_node(&params, node);
	if (last)
		ft_lstclear((t_list **) &last->next, free);
	else
		ft_lstclear((t_list **) &params.redirs, free);
	return (0);
}

int	cmd_pipe(t_cmd_params params, t_parse_node	*node)
{
	t_pipe			p;
	int				retval;
	t_redir			redir;

	if (ms_pipe(&p) < 0)
		return (MS_CMD_ERROR_PIPE | MS_CMD_ERROR_FAILURE);
	params.context |= MS_CMD_CONTEXT_PIPE;
	retval = 0;
	redir = (t_redir){
		.src = (t_redir_src){.type = MS_REDIR_FD, .fd = p.write},
		.dest = (t_redir_dest){.type = MS_REDIR_FD, .fd = STDOUT_FILENO}
	};
	if (_handle_pipe_end(&retval, params, redir, node->children[0]))
		return (MS_CMD_ERROR_PIPE);
	redir = (t_redir){
		.src = (t_redir_src){.type = MS_REDIR_FD, .fd = p.read},
		.dest = (t_redir_dest){.type = MS_REDIR_FD, .fd = STDIN_FILENO}
	};
	if (_handle_pipe_end(&retval, params, redir, node->children[1]))
		return (MS_CMD_ERROR_PIPE);
	return (retval);
}

int	cmd_next_node(t_cmd_params *params, t_parse_node *node)
{
	t_ms_token_type	type;
	t_ms_ops		op;

	type = node->tok.type;
	if (type == MS_TOK_ERROR || type != MS_TOK_OP)
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
