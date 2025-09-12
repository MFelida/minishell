/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/08/29 14:36:34 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"
#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int	_validate_input(char **args, t_cmd_params	*params)
{
	size_t	i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3, "minishell", "cd", _invalid_option(args[i], ""));
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

static void	_process_dotdot(char *path)
{
	char *dotdot;
	char *rest;

	dotdot = ft_strnstr(path, "/..", PATH_MAX);
	while (dotdot++)
	{
		if (dotdot[2] != '/' && dotdot[2] != '\0')
		{
			dotdot = ft_strnstr(dotdot + 2, "/..", PATH_MAX - (path - dotdot) - 2);
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
		printf("%s\n", path);
		dotdot = ft_strnstr(path, "/..", PATH_MAX);
	}
}

static void _process_dot(char *path)
{
	char	*dot;
	char	*rest;

	dot = ft_strnstr(path, "/.", PATH_MAX);
	while (dot++)
	{
		if (dot[1] != '/' && dot[1] != '\0')
		{
			dot = ft_strnstr(dot + 1, "/.", PATH_MAX - (path - dot) - 2);
			continue ;
		}
		if (dot[1])
			rest = dot + 2;
		else
		{
			rest = "";
			dot--;
		}
		ft_strlcpy(dot, rest, PATH_MAX + 1 - (path - dot));
		printf("%s\n", path);
		dot = ft_strnstr(dot, "/.", PATH_MAX - (path - dot));
	}
}

// TODO: Need to handle symbolic links ourselves, and don't forget to set $OLDPWD
int	ms_cd(char **args, t_cmd_params *params, ...)
{
	char	path[PATH_MAX + 1];

	if (_validate_input(args, params))
		return (MS_CMD_ERROR_OK);
	if (args[1] == NULL)
		ft_strlcpy(path, ms_getenv("HOME"), PATH_MAX + 1);
	else if (*args[1] == '/')
		ft_strlcpy(path, args[1], PATH_MAX + 1);
	else
	{
		ft_strlcpy(path, ms_getenv("PWD"),PATH_MAX + 1);
		ft_strlcat(path, "/", PATH_MAX + 1);
		ft_strlcat(path, args[1], PATH_MAX + 1);
	}
	printf("%s\n", path);
	_process_dot(path);
	_process_dotdot(path);
	if (chdir(path))
	{
		ft_print_err(strerror(errno), 2, "minishell", "cd");
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
	}
	else
		params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	ms_setenv("OLDPWD", ms_getenv("PWD"));
	ms_setenv("PWD", path);
	// TODO: DELETE
	printf("OLDPWD: %s\nPWD: %s\n", ms_getenv("OLDPWD"), ms_getenv("PWD"));
	return (MS_CMD_ERROR_OK);
}
