/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:04:32 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 17:24:39 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

t_lex_tok	*lx_new_id_tok(t_lex_context *context)
{
	t_lex_tok	*res;

	res = lx_default_tok();
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

void	lx_handle_var(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new;

	context->error++;
	if (*context->start != '$' || context->curr != context->start)
		return ;
	if (context->start[1] == '_' || ft_isalpha(context->start[1])
			|| !ft_strncmp(context->start, "$?", 2))
		context->start++;
	context->curr = context->start + 1;
	while (*context->curr
		&& *(context->curr - 1) != '?'
		&& (ft_isalnum(*context->curr) || *context->curr == '_'))
		context->curr++;
	new = lx_new_id_tok(context);
	context->start = context->curr;
	if (!new)
	{
		ft_print_err(strerror(errno), 2, "minishell", __FUNCTION__);
		return ;
	}
	if (new->id[0] != '$')
		new->type = MS_LEX_TOK_VAR;
	ft_lstadd_back((t_list **) lex_list, (t_list *) new);
	context->start = context->curr;
	context->error--;
}

t_lex_tok	*lex_input(char *input)
{
	t_lex_tok		*res;
	t_lex_context	context;

	res = NULL;
	context = (t_lex_context){.input = input, .start = input,
		.curr = input, .error = 0, .quote_state = MS_LEX_QUOTE_NONE};
	while (*context.start && !context.error)
	{
		if (context.quote_state == MS_LEX_QUOTE_SINGLE)
			lx_handle_single(&res, &context);
		else if (*context.start == '$')
			lx_handle_var(&res, &context);
		else if (context.quote_state == MS_LEX_QUOTE_DOUBLE)
			lx_handle_double(&res, &context);
		else if (context.quote_state == MS_LEX_QUOTE_NONE)
			lx_handle_unquoted(&res, &context);
	}
	free(input);
	if (res && (context.quote_state != MS_LEX_QUOTE_NONE || context.error))
	{
		free_lex_list(&res);
		return (NULL);
	}
	return (res);
}
