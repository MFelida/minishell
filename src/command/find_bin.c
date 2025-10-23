/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:04:42 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 13:54:38 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exit_statuses.h"
#include "libft.h"

#include <asm-generic/errno-base.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>

static char	*_build_path(char *dest, const char *path, const char *file)
{
	if (ft_strlcpy(dest, path, PATH_MAX) >= PATH_MAX)
		return (NULL);
	if (ft_strlcat(dest, "/", PATH_MAX) >= PATH_MAX)
		return (NULL);
	if (ft_strlcat(dest, file, PATH_MAX) >= PATH_MAX)
		return (NULL);
	return (dest);
}

static int	_find_on_path(char	*dest, const char *name)
{
	const char	*path_var;
	char		**path_split;
	int			i;

	path_var = ms_getenv("PATH");
	if (!path_var || *name == '\0')
		return (MS_CMD_NOT_FOUND);
	path_split = ft_split(path_var, ':');
	if (!path_split)
		return (MS_UNEXPECTED);
	i = -1;
	while (dest && path_split[++i])
	{
		dest = _build_path(dest, path_split[i], name);
		if (!dest || access(dest, F_OK))
			continue ;
		ft_split_free(path_split);
		if (access(dest, X_OK))
			return (MS_PERM_DENIED);
		return (0);
	}
	ft_split_free(path_split);
	if (!dest)
		return (MS_UNEXPECTED);
	return (MS_CMD_NOT_FOUND);
}

int	is_rel_path(const char *str)
{
	if (ft_strchr(str, '/'))
		return (2);
	if (ft_strchr(str, '.'))
		return (1);
	return (0);
}

int	find_bin(char *dest, const char *name)
{
	struct stat	stat_buff;

	if (is_rel_path(name))
	{
		dest[0] = '\0';
		if (!ft_strchr(name, '/'))
			ft_strlcat(dest, "./", PATH_MAX);
		ft_strlcat(dest, name, PATH_MAX);
		if (access(dest, F_OK))
		{
			if (!ft_strchr(name, '/'))
				return (_find_on_path(dest, name));
			return (MS_CMD_NOT_FOUND);
		}
		if (access(dest, X_OK) || ((stat(dest, &stat_buff) == 0)
				&& S_ISDIR(stat_buff.st_mode)))
			return (MS_PERM_DENIED);
		return (0);
	}
	return (_find_on_path(dest, name));
}
