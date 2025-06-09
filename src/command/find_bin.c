/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:04:42 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/02 17:56:32 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	_find_on_path(char	*dest, const char *name)
{
	char		*path_var;
	char		**path_split;
	int			i;

	path_var = getenv("PATH");
	if (!path_var)
		return (1);
	path_split = ft_split(path_var, ':');
	i = 0;
	while (path_split[i])
	{
		ft_strlcpy(dest, path_split[i], PATH_MAX);
		ft_strlcat(dest, "/", PATH_MAX);
		ft_strlcat(dest, name, PATH_MAX);
		if (!access(dest, F_OK))
		{
			ft_split_free(path_split);
			return (access(dest, X_OK));
		}
		i++;
	}
	ft_split_free(path_split);
	return (1);
}

int	find_bin(char *dest, const char *name)
{
	if (ft_strchr(name, '/'))
	{
		ft_strlcpy(dest, name, PATH_MAX);
		if (!access(dest, F_OK))
			return (access(dest, X_OK));
		return (1);
	}
	return (_find_on_path(dest, name));
}
