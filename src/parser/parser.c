/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:05:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 15:49:45 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "hear_doc.h"
#include "libft.h"
#include "parse_tree.h"
#include "parser/lexer.h"
#include "parser.h"
#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

t_parse_node	*get_next_node(t_lex_tok **lex_list, t_parse_context *context);
void			fp_print(t_parse_node node, int tab_depth);

t_parse_node	*get_op_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_lex_tok					*tok;
	static const t_node_getter	op_node_getters[] = {
		NULL,
	[MS_LEX_OP_INPUT] = get_input_node,
	[MS_LEX_OP_HEREDOC] = get_heredoc_node,
	[MS_LEX_OP_OUTPUT] = get_output_node,
	[MS_LEX_OP_APPEND] = get_append_node,
	[MS_LEX_OP_PIPE] = get_pipe_node,
	};

	tok = *lex_list;
	if (tok->op == MS_LEX_OP_ERROR)
		return (NULL);
	return (op_node_getters[tok->op](lex_list, context));
}

t_parse_node	*get_id_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*tok;

	res = ft_calloc(1, sizeof(t_parse_node));
	if (!res)
		return (NULL);
	tok = *lex_list;
	res->tok = (t_ms_token){.id.type = MS_TOK_IDENTIFIER,
		.id.value = ft_strdup(tok->id)};
	if (!res->tok.id.value)
	{
		free(res);
		free_parse_tree(&context->root);
		return (NULL);
	}
	*lex_list = tok->next;
	return (res);
}

t_parse_node	*get_ws_node(t_lex_tok **lex_list, t_parse_context *context);

t_parse_node	*get_var_node(t_lex_tok **lex_list, t_parse_context *context);

static int	_get_parse_tree(t_lex_tok **lex_list, t_parse_context *context)
{
	t_lex_tok		*curr;
	t_parse_node	*new;

	curr = *lex_list;
	while (curr && curr->type == MS_LEX_TOK_WS)
		curr = curr->next;
	if (curr && curr->type == MS_LEX_TOK_OP && curr->op == MS_LEX_OP_PIPE)
		return (ft_print_err(parser_strerror(curr), 1, "minishell"), 1);
	curr = *lex_list;
	while (curr)
	{
		if (curr->type == MS_LEX_TOK_ERROR)
			return (free_parse_tree(&context->root), 1);
		else if (context->root == NULL)
			new = get_cmd_node(&curr, context);
		else if (curr->type == MS_LEX_TOK_OP && curr->op == MS_LEX_OP_PIPE)
			new = get_pipe_node(&curr, context);
		else
			return (free_parse_tree(&context->root), 1);
		if (new == NULL)
			return (free_parse_tree(&context->root), 1);
	}
	return (0);
}

static t_parse_context	_init_context(t_parse_context *context)
{
	context->root = NULL;
	context->current_cmd_node = &context->root;
	return (*context);
}

t_parse_node	*get_parse_tree(char *input)
{
	t_lex_tok		*lex_list;
	t_parse_context	context;

	lex_list = lex_input(input);
	if (!lex_list)
		return (NULL);
	expand_vars(lex_list);
	_init_context(&context);
	if (_get_parse_tree(&lex_list, &context))
		free_parse_tree(&context.root);
	free_lex_list(&lex_list);
	return (context.root);
}
