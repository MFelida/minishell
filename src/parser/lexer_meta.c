/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_meta.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:10:33 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 16:29:21 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static t_lex_tok	*lx_new_op_tok(t_lex_context *context)
{
	t_lex_tok	*new;

	new = ft_calloc(1, sizeof(t_lex_tok));
	if (!new)
		return (NULL);
	new->type = MS_LEX_TOK_OP;
	if (*context->curr == '<')
		new->op = MS_LEX_OP_INPUT + (context->curr[1] == '<');
	else if (*context->curr == '>')
		new->op = MS_LEX_OP_OUTPUT + (context->curr[1] == '>');
	else if (*context->curr == '|')
		new->op = MS_LEX_OP_PIPE;
	else
		context->error = 1;
	context->curr += 1 + (new->op == MS_LEX_OP_HEREDOC
			|| new->op == MS_LEX_OP_APPEND);
	context->start = context->curr;
	return (new);
}

void	lx_handle_meta(t_lex_tok **lex_list, t_lex_context *context)
{
	t_lex_tok	*new;

	context->error++;
	if (is_ws(*context->curr))
	{
		new = lx_default_tok();
		if (!new)
			return ;
		new->type = MS_LEX_TOK_WS;
		context->curr = skip_ws(context->curr);
		context->start = context->curr;
	}
	else
	{
		new = lx_new_op_tok(context);
		if (!new)
			return ;
	}
	ft_lstadd_back((t_list **) lex_list, (t_list *) new);
	context->error--;
}
