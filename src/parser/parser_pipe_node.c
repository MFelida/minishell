/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:23:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 14:27:45 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parse_tree.h"
#include "parser.h"
#include "utils.h"

#include <stdlib.h>

t_parse_node	*get_pipe_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*next_tok;

	res = new_node(3);
	if (!res)
		return (NULL);
	res->children[0] = context->root;
	context->root = res;
	context->current_cmd_node = &res->children[1];
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_PIPE};
	*lex_list = (*lex_list)->next;
	next_tok = *lex_list;
	while (next_tok && next_tok->type == MS_LEX_TOK_WS)
		next_tok = next_tok->next;
	if (!next_tok
		|| (next_tok->type == MS_LEX_TOK_OP && next_tok->op == MS_LEX_OP_PIPE))
	{
		ft_print_err(parser_strerror(next_tok), 1, "minishell");
		return (free_parse_tree(&context->root), NULL);
	}
	if (!get_cmd_node(lex_list, context) || !res->children[1])
		return (free_parse_tree(&context->root), NULL);
	return (res);
}
