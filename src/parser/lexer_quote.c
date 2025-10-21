/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:03:39 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 16:18:01 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static void	_set_quote_state(t_lex_context *context)
{
	if (!is_quote(*context->curr))
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

void	lx_handle_single(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr && *context->curr != '\'')
		context->curr++;
	new_node = lx_new_id_tok(context);
	if (!new_node)
		return ;
	ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
	context->start = context->curr;
	if (is_quote(*context->curr))
		_set_quote_state(context);
}

void	lx_handle_unquoted(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr
		&& !is_meta(*context->curr) && !is_quote(*context->curr)
		&& *context->curr != '$')
		context->curr++;
	if (context->curr != context->start)
	{
		new_node = lx_new_id_tok(context);
		if (!new_node)
			return ;
		ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
		context->start = context->curr;
	}
	if (is_meta(*context->curr))
		lx_handle_meta(lex_list, context);
	else if (is_quote(*context->curr))
		_set_quote_state(context);
}

void	lx_handle_double(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new_node;

	while (*context->curr && *context->curr != '"' && *context->curr != '$')
		context->curr++;
	new_node = lx_new_id_tok(context);
	if (!new_node)
		return ;
	ft_lstadd_back((t_list **) lex_list, (t_list *) new_node);
	context->start = context->curr;
	if (is_quote(*context->curr))
		_set_quote_state(context);
}
