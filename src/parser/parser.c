/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:05:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/29 10:31:49 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "parse_tree.h"
#include "libft.h"
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

int	count_chld_nodes(t_parse_node *parent)
{
	int				res;
	t_parse_node	**chld_arr;

	res = 0;
	chld_arr = parent->children;
	while (*(chld_arr++) != NULL)
		res++;
	return (res);
}

typedef	t_parse_node	*(*t_node_getter)(t_lex_tok **, t_parse_context *context);

static t_parse_node	*_get_redir_node(t_lex_tok **lex_list, t_parse_context *context)
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
			ft_print_err("syntax error near unexpected token `newline'", 1, "minishell");
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

t_parse_node	*get_heredoc_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;

	res = _get_redir_node(lex_list, context);
	if (!res)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_HEREDOC};
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

t_parse_node	*get_pipe_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*next_tok;

	res = malloc(sizeof(t_parse_node));
	if (!res)
		return (NULL);
	res->children = ft_calloc(3, sizeof(t_parse_node));
	if (!res->children)
		return (NULL);
	res->children[0] = context->root;
	context->root = res;
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_PIPE};
	context->current_cmd_node = &res->children[1];
	*lex_list = (*lex_list)->next;
	next_tok = *lex_list;
	while (next_tok && next_tok->type == MS_LEX_TOK_WS)
		next_tok = next_tok->next;
	if (!next_tok || (next_tok->type == MS_LEX_TOK_OP && next_tok->op == MS_LEX_OP_PIPE))
	{
		if (!next_tok)
			ft_print_err("syntax error near unexpected token `newline'", 1, "minishell");
		else
			ft_print_err(parser_strerror(next_tok), 1, "minishell");
		return (free_parse_tree(&context->root), NULL);
	}
	if (!get_cmd_node(lex_list, context) || !res->children[1])
		return (free_parse_tree(&context->root), NULL);
	return (res);
}

static t_node_getter	g_op_node_getters[] = {
	[MS_LEX_OP_INPUT] = get_input_node,
	[MS_LEX_OP_HEREDOC] = get_heredoc_node,
	[MS_LEX_OP_OUTPUT] = get_output_node,
	[MS_LEX_OP_APPEND] = get_append_node,
	[MS_LEX_OP_PIPE] = get_pipe_node,
};

t_parse_node	*get_op_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_lex_tok	*tok;

	tok = *lex_list;
	if (tok->op == MS_LEX_OP_ERROR)
		return (NULL);
	return (g_op_node_getters[tok->op](lex_list, context));
}

t_parse_node	*get_id_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_lex_tok		*tok;

	res = ft_calloc(1, sizeof(t_parse_node));
	if (!res)
		return (NULL);
	tok = *lex_list;
	res->tok = (t_ms_token){.id.type = MS_TOK_IDENTIFIER, .id.value = ft_strdup(tok->id)};
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

// static t_node_getter g_node_getters[] = {
// 	[MS_LEX_TOK_OP] = get_op_node,
// 	[MS_LEX_TOK_ID] = get_id_node,
// 	[MS_LEX_TOK_WS] = get_ws_node,
// 	[MS_LEX_TOK_VAR]= get_var_node,
// };

size_t	_count_children(t_lex_tok	*lext_list)
{
	size_t	i;

	i = 0;
	while (lext_list && !(lext_list->type == MS_LEX_TOK_OP && lext_list->op == MS_LEX_OP_PIPE))
	{
		lext_list = lext_list->next;
		i++;
	}
	return (i);
}

t_parse_node	*get_cmd_node(t_lex_tok **lex_list, t_parse_context *context)
{
	t_parse_node	*res;
	t_parse_node	*parent;
	t_lex_tok		*tok;
	size_t			i;

	res = ft_calloc(1, sizeof(t_parse_node));
	if (!res)
		return (NULL);
	res->children = ft_calloc(_count_children(*lex_list), sizeof(t_parse_node));
	if (!res->children)
		return (NULL);
	res->tok = (t_ms_token){.op.type = MS_TOK_OP, .op.op = MS_OP_CMD};
	*context->current_cmd_node = res;
	i = 0;
	tok = *lex_list;
	while (tok && !(tok->type == MS_LEX_TOK_OP && tok->op == MS_LEX_OP_PIPE))
	{
		if (tok->type == MS_LEX_TOK_ID)
		{
			res->children[i] = get_id_node(&tok, context);
			if (!res->children[i++])
				return (free_parse_tree(&context->root), NULL);
		}
		else if (tok->type == MS_LEX_TOK_OP)
		{
			parent = g_op_node_getters[tok->op](&tok, context);
			if (!parent)
				return (NULL);
			*context->current_cmd_node = parent;
			parent->children[1] = res;
			context->current_cmd_node = &parent->children[1];
		}
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

static int	_get_parse_tree(t_lex_tok **lex_list, t_parse_context *context)
{
	t_lex_tok		*curr;
	t_parse_node	*new;

	curr = *lex_list;
	while (curr && curr->type == MS_LEX_TOK_WS)
		curr = curr->next;
	if (curr && curr->type == MS_LEX_TOK_OP && curr->op == MS_LEX_OP_PIPE)
	{
		ft_print_err(parser_strerror(curr), 1, "minishell");
		return (1);
	}
	curr = *lex_list;
	while (curr)
	{
		if (curr->type == MS_LEX_TOK_ERROR)
			return (1);
		else if (context->root == NULL)
		{
			new = get_cmd_node(&curr, context);
		}
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

void	free_parse_tree(t_parse_node **root)
{
	size_t			i;
	t_parse_node	*node;

	if (!root || !*root)
		return ;
	i = 0;
	node = *root;
	while (node->children && node->children[i])
		free_parse_tree(&node->children[i++]);
	if (node->tok.type == MS_TOK_IDENTIFIER)
		free(node->tok.id.value);
	if (node->children)
		free(node->children);
	free(node);
	*root = NULL;
}
