/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exit_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:34:44 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 13:35:07 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

#include <stdlib.h>

const char	*_exit_status(const char *value)
{
	static char	exit_status[10] = "0";

	if (!value)
		return (exit_status);
	ft_strlcpy(exit_status, value, 10);
	return (exit_status);
}

int	ms_set_exitstatus(const int status)
{
	char	*str;

	str = ft_itoa(status);
	if (!str)
		return (1);
	ms_setenv("?", str);
	free(str);
	return (0);
}
