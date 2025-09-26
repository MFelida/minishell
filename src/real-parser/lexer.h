/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:52:14 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/26 17:16:15 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct	s_lex_lst
{
	struct s_lex_lst	*next;
	char				*str;
}	t_lex_lst;

t_lex_lst	*lex_input(char *input);
void		free_lex_list(t_lex_lst *lst);
