/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 15:31:16 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 16:05:26 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "lexer.h"
#include "libft.h"

#include <stdlib.h>

static void	_concat_identifiers(t_lex_tok *lex_list)
{
	t_lex_tok	*start;
	t_lex_tok	*curr;
	char		*temp;

	start = lex_list;
	while (start)
	{
		while (start && start->type != MS_LEX_TOK_ID)
			start = start->next;
		if (!start)
			return ;
		curr = start->next;
		while (curr && curr->type == MS_LEX_TOK_ID)
		{
			temp = start->id;
			start->id = ft_strjoin(temp, curr->id);
			free(temp);
			start->next = curr->next;
			free_lex_node(curr);
			curr = start->next;
		}
		start = start->next;
	}
}

t_lex_tok	*expand_vars(t_lex_tok *lex_list)
{
	t_lex_tok	*curr;
	char		*temp;

	curr = lex_list;
	while (curr)
	{
		if (curr->type == MS_LEX_TOK_VAR)
		{
			temp = curr->id;
			curr->type = MS_LEX_TOK_ID;
			if (!ms_getenv(temp) || ms_getenv(temp)[0] == '\0')
			{
				curr->id = ft_strdup("");
				if (!curr->was_quoted)
					curr->type = MS_LEX_TOK_WS;
			}
			else
				curr->id = ft_strdup(ms_getenv(temp));
			free(temp);
		}
		curr = curr->next;
	}
	_concat_identifiers(lex_list);
	return (lex_list);
}
