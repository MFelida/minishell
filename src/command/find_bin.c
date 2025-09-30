/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:04:42 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 17:58:12 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exit_statuses.h"
#include "libft.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <linux/limits.h>
#include <unistd.h>

static int	_find_on_path(char	*dest, const char *name)
{
	const char	*path_var;
	char	**path_split;
	int		i;

	path_var = ms_getenv("PATH");
	if (!path_var)
		return (1);
	path_split = ft_split(path_var, ':');
	if (!path_split)
		return (1);
	i = -1;
	while (path_split[++i])
	{
		ft_strlcpy(dest, path_split[i], PATH_MAX);
		ft_strlcat(dest, "/", PATH_MAX);
		ft_strlcat(dest, name, PATH_MAX);
		if (access(dest, F_OK))
			continue ;
		ft_split_free(path_split);
		return (access(dest, X_OK));
	}
	ft_split_free(path_split);
	return (MS_CMD_NOT_FOUND);
}

int	find_bin(char *dest, const char *name)
{
	if (ft_strchr(name, '/'))
	{
		ft_strlcpy(dest, name, PATH_MAX);
		if (access(dest, F_OK) || access(dest, X_OK))
			return (1);
		return (0);
	}
	return (_find_on_path(dest, name));
}
