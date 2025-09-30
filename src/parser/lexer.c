/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:04:32 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/29 10:11:47 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

#define METACHARS	" \t\n|&;()<>"
#define WS			" \t\n"

static t_lex_tok	*_default_tok(void)
{
	t_lex_tok	*res;

	res = ft_calloc(1, sizeof(t_lex_tok));
	if (!res)
		return (NULL);
	return (res);
}

void	free_lex_node(void *node)
{
	t_lex_tok	*tok;

	tok = node;
	if (tok->id)
		free(tok->id);
	free(node);
}

void	free_lex_list(t_lex_tok **lex_list)
{
	ft_lstclear((t_list **) lex_list, free_lex_node);
}

static int	_is_meta(const char c)
{
	return (c && !!ft_strchr(METACHARS, c));
}

static int	_is_quote(const char c)
{
	return (c && !!ft_strchr("'\"", c));
}

static int	_is_ws(const char c)
{
	return (c && !!ft_strchr(WS, c));
}

static char	*_skip_ws(char *s)
{
	while (_is_ws(*s))
		s++;
	return (s);
}

static t_lex_tok	*_new_id_tok(t_lex_context *context)
{
	t_lex_tok	*res;

	res = _default_tok();
	if (!res)
	{
		context->error = 1;
		return (res);
	}
	res->type = MS_LEX_TOK_ID;
	res->id = ft_substr(context->start, 0, context->curr - context->start);
	if (!res->id)
	{
		free(res);
		context->error = 1;
		return (NULL);
	}
	return (res);
}

static t_lex_tok	*_new_op_tok(t_lex_context *context)
{
	t_lex_tok	*new;

	new = ft_calloc(1, sizeof(t_lex_tok));
	if (!new)
		return (NULL);
	new->type = MS_LEX_TOK_OP;
	if (*context->curr == '<')
	{
		new->op = MS_LEX_OP_INPUT;
		if (context->curr[1] == '<')
			new->op = MS_LEX_OP_HEREDOC;
	}
	else if (*context->curr == '>')
	{
		new->op = MS_LEX_OP_OUTPUT;
		if (context->curr[1] == '>')
			new->op = MS_LEX_OP_APPEND;
	}
	else if (*context->curr == '|')
		new->op = MS_LEX_OP_PIPE;
	else
		context->error = 1;
	context->curr++;
	if (new->op == MS_LEX_OP_APPEND || new->op == MS_LEX_OP_HEREDOC)
		context->curr++;
	context->start = context->curr;
	return (new);
}

static void	_handle_meta(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new;

	context->error++;
	if (_is_ws(*context->curr))
	{
		new = _default_tok();
		if (!new)
			return ;
		new->type = MS_LEX_TOK_WS;
		context->curr = _skip_ws(context->curr);
		context->start = context->curr;
	}
	else
	{
		new = _new_op_tok(context);
		if (!new)
			return ;
	}
	ft_lstadd_back((t_list **) lex_list, (t_list *) new);
	context->error--;
}

static void	_handle_var(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new;

	context->error++;
	if (*context->start != '$'
		|| context->curr != context->start
		|| !(context->start[1] == '_' || ft_isalpha(context->start[1])
		|| !ft_strncmp(context->start, "$?", 3)))
	{
		return ;
	}
	context->start++;
	context->curr++;
	if (*context->curr == '?')
		context->curr++;
	else
		while (*context->curr && ft_isalnum(*context->curr))
			context->curr++;
	new = _new_id_tok(context);
	context->start = context->curr;
	if (!new)
	{
		context->error = 1;
		return ;
	}
	new->type = MS_LEX_TOK_VAR;
	ft_lstadd_back((t_list **) lex_list, (t_list *) new);
	context->start = context->curr;
	context->error--;
}

static void	_set_quote_state(t_lex_context *context)
{
	if (!_is_quote(*context->curr))
		return ;
	if (context->quote_state == MS_LEX_QUOTE_NONE)
	{
		if (*context->curr == '\'')
			context->quote_state = MS_LEX_QUOTE_SINGLE;
		if (*context->curr == '"')
			context->quote_state = MS_LEX_QUOTE_DOUBLE;
	}
	else if (context->quote_state == MS_LEX_QUOTE_SINGLE
			&& *context->curr == '\'')
		context->quote_state = MS_LEX_QUOTE_NONE;
	else if (context->quote_state == MS_LEX_QUOTE_DOUBLE
			&& *context->curr == '"')
		context->quote_state = MS_LEX_QUOTE_NONE;
	context->curr++;
	context->start++;
}

static void	_handle_unquoted(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr
		&& !_is_meta(*context->curr) && !_is_quote(*context->curr)
		&& *context->curr != '$')
		context->curr++;
	if (context->curr != context->start)
	{
		new_node = _new_id_tok(context);
		if (!new_node)
			return ;
		ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
		context->start = context->curr;
	}
	if (_is_meta(*context->curr))
		_handle_meta(lex_list, context);
	else if (_is_quote(*context->curr))
		_set_quote_state(context);
	else if (*context->curr == '$')
		return ;
}

static void	_handle_single(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr && *context->curr != '\'')
		context->curr++;
	if (context->curr != context->start)
	{
		new_node = _new_id_tok(context);
		if (!new_node)
			return ;
		ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
		context->start = context->curr;
	}
	if (_is_quote(*context->curr))
		_set_quote_state(context);
}

static void	_handle_double(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr && *context->curr != '"' && *context->curr != '$')
		context->curr++;
	if (context->curr != context->start)
	{
		new_node = _new_id_tok(context);
		if (!new_node)
			return ;
		ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
		context->start = context->curr;
	}
	if (_is_quote(*context->curr))
		_set_quote_state(context);
	else if (*context->curr == '$')
		return ;
}

t_lex_tok	*lex_input(char *input)
{
	t_lex_tok		*res;
	t_lex_context	context;

	res = NULL;
	context = (t_lex_context) {.input = input, .start = input,
		.curr = input, .error = 0, .quote_state = MS_LEX_QUOTE_NONE};
	while (*context.start && !context.error)
	{
		if (context.quote_state == MS_LEX_QUOTE_SINGLE)
			_handle_single(&res, &context);
		else if (*context.start == '$')
			_handle_var(&res, &context);
		else if (context.quote_state == MS_LEX_QUOTE_DOUBLE)
			_handle_double(&res, &context);
		else if (context.quote_state == MS_LEX_QUOTE_NONE)
			_handle_unquoted(&res, &context);
	}
	free(input);
	if (res && (context.quote_state != MS_LEX_QUOTE_NONE || context.error))
	{
		free_lex_list(&res);
		return (NULL);
	}
	// for (t_lex_tok *t = res; t != NULL; t = t->next)
	// {
	// 	if (t->type == MS_LEX_TOK_WS)
	// 		printf("WS\n");
	// 	if (t->type == MS_LEX_TOK_ID || t->type == MS_LEX_TOK_VAR)
	// 		printf("type: %d, value: %s\n", t->type, t->id);
	// 	if (t->type == MS_LEX_TOK_OP)
	// 		printf("type: %d, op: %d\n", t->type, t->op);
	// }
	return (res);
}
