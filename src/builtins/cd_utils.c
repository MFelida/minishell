/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:20:21 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/15 17:23:53 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>
#include <stdio.h>

int	_validate_input(char **args, t_cmd_params	*params)
{
	size_t	i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3,
				"minishell", "cd", _invalid_option(args[i], ""));
			params->wstatus = _set_wstatus(MS_BUILTIN_MISUSE, 0);
			return (MS_BUILTIN_MISUSE);
		}
	}
	if (i > 2)
	{
		ft_print_err("too many arguments", 2, "minishell", "cd");
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
		return (MS_FAILURE);
	}
	return (MS_SUCCESS);
}

void	_process_dotdot(char *path)
{
	char	*dotdot;
	char	*rest;

	dotdot = ft_strnstr(path, "/..", PATH_MAX);
	while (dotdot++)
	{
		if (dotdot[2] != '/' && dotdot[2] != '\0')
		{
			dotdot = ft_strnstr(
					dotdot + 2, "/..", PATH_MAX - (path - dotdot) - 2);
			continue ;
		}
		if (dotdot[2])
			rest = dotdot + 3;
		else
			rest = "";
		if (dotdot > path + 1)
		{
			dotdot[-1] = '\0';
			dotdot = ft_strrchr(path, '/') + 1;
		}
		ft_strlcpy(dotdot, rest, PATH_MAX - (path - dotdot));
		dotdot = ft_strnstr(path, "/..", PATH_MAX);
	}
}

void	_process_dot(char *path)
{
	char	*dot;
	char	*rest;

	dot = ft_strnstr(path, "/.", PATH_MAX);
	while (dot)
	{
		if (dot[2] != '/' && dot[2] != '\0')
		{
			dot = ft_strnstr(dot + 2, "/.", PATH_MAX - (path - dot) - 3);
			continue ;
		}
		if (dot[2])
			rest = dot + 3;
		else
			rest = "";
		dot++;
		ft_strlcpy(dot, rest, PATH_MAX - (dot - path));
		dot = ft_strnstr(dot, "/.", PATH_MAX - (dot - path));
	}
}
