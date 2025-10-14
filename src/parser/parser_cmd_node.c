/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:55:01 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/14 15:40:12 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "parse_tree.h"
#include "parser.h"
#include "parser/lexer.h"
#include "parser/utils.h"

#include <stddef.h>

static size_t	_count_children(t_lex_tok	*lext_list)
{
	size_t	i;

	i = 0;
	while (lext_list && !(lext_list->type == MS_LEX_TOK_OP
			&& lext_list->op == MS_LEX_OP_PIPE))
	{
		lext_list = lext_list->next;
		i++;
	}
	return (i);
}

static t_parse_node	*_get_parent(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*parent;

	parent = get_op_node(lex_list, context);
	if (!parent)
		return (NULL);
	parent->children[1] = *context->current_cmd_node;
	*context->current_cmd_node = parent;
	context->current_cmd_node = &parent->children[1];
	return (parent);
}

static t_parse_node	*_get_id(t_parse_node **dest,
							t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*new;

	*dest = NULL;
	new = get_id_node(lex_list, context);
	if (!new)
		return (NULL);
	*dest = new;
	return (new);
}

t_parse_node	*get_cmd_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*tok;
	size_t			i;

	res = new_node(_count_children(*lex_list));
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_CMD};
	*context->current_cmd_node = res;
	i = 0;
	tok = *lex_list;
	while (tok && !(tok->type == MS_LEX_TOK_OP && tok->op == MS_LEX_OP_PIPE))
	{
		if (tok->type == MS_LEX_TOK_ID
			&& _get_id(&res->children[i++], &tok, context))
			return (free_parse_tree(&context->root), NULL);
		else if (tok->type == MS_LEX_TOK_OP && !_get_parent(&tok, context))
			return (free_parse_tree(&context->root), NULL);
		else if (tok->type == MS_LEX_TOK_WS)
			tok = tok->next;
		else
			return (free_parse_tree(&context->root), NULL);
	}
	*lex_list = tok;
	if (res->children[0] && is_builtin(res->children[0]->tok.id.value))
		res->tok.op.op = MS_OP_BLTIN;
	return (res);
}
