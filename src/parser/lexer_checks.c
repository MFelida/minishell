/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:58:03 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 16:18:10 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define METACHARS	" \t\n|&;()<>"
#define WS			" \t\n"

int	is_meta(const char c)
{
	return (c && !!ft_strchr(METACHARS, c));
}

int	is_quote(const char c)
{
	return (c && !!ft_strchr("'\"", c));
}

int	is_ws(const char c)
{
	return (c && !!ft_strchr(WS, c));
}

char	*skip_ws(char *s)
{
	while (is_ws(*s))
		s++;
	return (s);
}
