/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:59:36 by amel-fou          #+#    #+#             */
/*   Updated: 2025/06/24 11:08:46 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HEADER_H
# define PARSING_HEADER_H
#include "tokens.h"

int	ft_isspace(char c);

typedef struct s_token_list
{
	char *string;
	int *type;
	t_token_list *next;
}	t_token_list;

size_t	ft_strlcpy(char *dst, const char *src, size_t size);

#endif