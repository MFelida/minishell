/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:06:13 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 15:31:53 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <unistd.h>

int	_is_option(const char *arg)
{
	return (*arg == '-');
}

const char	*_invalid_option(const char *arg, const char *valid_opts)
{
	static char buff[3] = "-\0";

	while (*++arg)
	{
		if (!ft_strchr(valid_opts, *arg))
		{
			buff[1] = *arg;
			return (buff);
		}
	}
	return (NULL);
}
