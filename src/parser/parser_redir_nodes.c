/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input_nodes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:11:02 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/14 15:15:23 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hear_doc.h"
#include "libft.h"
#include "parse_tree.h"
#include "lexer.h"
#include "parser.h"
#include "utils.h"

#include <stdlib.h>

static t_parse_node	*_get_redir_node(t_lex_tok **lex_list,
									t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*tok;

	res = ft_calloc(1, sizeof(t_parse_node));
	if (!res)
		return (NULL);
	res->children = ft_calloc(3, sizeof(t_parse_node));
	if (!res->children)
		return (NULL);
	tok = *lex_list;
	tok = tok->next;
	while (tok && tok->type == MS_LEX_TOK_WS)
		tok = tok->next;
	if (!tok || tok->type != MS_LEX_TOK_ID)
	{
		if (!tok)
			ft_print_err("syntax error near unexpected token `newline'",
				1, "minishell");
		else
			ft_print_err(parser_strerror(tok), 1, "minishell");
		free(res->children);
		free(res);
		free_parse_tree(&context->root);
		return (NULL);
	}
	res->children[0] = get_id_node(&tok, context);
	if (!res->children[0])
		return (free_parse_tree(&context->root), NULL);
	*lex_list = tok;
	return (res);
}

t_parse_node	*get_input_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;

	res = _get_redir_node(lex_list, context);
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_FILE_INPUT};
	return (res);
}

t_parse_node	*get_heredoc_node(t_lex_tok **lex_list,
							t_parse_context *context)
{
	t_parse_node	*res;

	res = _get_redir_node(lex_list, context);
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_HEREDOC};
	res->tok.op.value = ms_hear_doc(res->children[0]->tok.id.value);
	if (res->tok.op.value == -1)
	{
		free_parse_tree(&context->root);
		return (NULL);
	}
	return (res);
}

t_parse_node	*get_output_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;

	res = _get_redir_node(lex_list, context);
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_FILE_OUTPUT};
	return (res);
}

t_parse_node	*get_append_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;

	res = _get_redir_node(lex_list, context);
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_FILE_APPEND};
	return (res);
}
