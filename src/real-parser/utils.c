/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:56:00 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/26 17:06:30 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define WS "\t\v\f "

int	is_ws(const char c)
{
	return (!!ft_strchr(WS, c));
}

char *skip_ws(const char *str)
{
	while (*str && is_ws(*str))
		str++;
	return ((char *) str);
}
