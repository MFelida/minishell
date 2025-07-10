/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:59:36 by amel-fou          #+#    #+#             */
/*   Updated: 2025/07/10 18:01:43 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HEADER_H
# define PARSING_HEADER_H
#include "tokens.h"

typedef enum argument_state
{
NOT_IN_QUOTES,
ESCAPING,
IN_SINGLE_QUOTE,
IN_DOUBLE_QUOTE,
} t_argument_state;

typedef enum quote_state
{
	NOT_ESCAPING,
	ESCAPING,
}	t_quote_state;

typedef enum double_quote_state
{
	NOT_ESCAPING,
	ESCAPING,
}	t_quote_state;

int	ft_isspace(char c);

typedef struct s_token_list
{
	t_token_list *next;
	char *string;
	int *type;
}	t_token_list;

size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_bzero(void *s, size_t n);

#endif