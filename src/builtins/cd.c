/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/15 17:33:35 by mifelida         ###   ########.fr       */
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

int		_validate_input(char **args, t_cmd_params	*params);
void	_process_dotdot(char *path);
void	_process_dot(char *path);

static char	*_get_pwd(char *dest)
{
	if (ms_getenv("PWD") && valid_pwd(ms_getenv("PWD")))
		ft_strlcpy(dest, ms_getenv("PWD"), PATH_MAX + 1);
	else if (!getcwd(dest, PATH_MAX))
	{
		ft_print_err(strerror(errno), 3, "minishell", "cd", "getcwd");
		return (NULL);
	}
	return (dest);
}

static int	_get_path(char *dest, const char *arg)
{
	if (arg == NULL)
	{
		if (ms_getenv("HOME") == NULL)
		{
			ft_print_err("HOME not set", 2, "minishell", "cd");
			return (MS_FAILURE);
		}
		ft_strlcpy(dest, ms_getenv("HOME"), PATH_MAX + 1);
	}
	else if (*arg == '/')
		ft_strlcpy(dest, arg, PATH_MAX + 1);
	else
	{
		if (!_get_pwd(dest))
			return (MS_FAILURE);
		if (*(ft_strchr(dest, '\0') - 1) != '/')
			ft_strlcat(dest, "/", PATH_MAX + 1);
		ft_strlcat(dest, arg, PATH_MAX + 1);
	}
	if (*(ft_strchr(dest, '\0') - 1) == '/' && ft_strlen(dest) > 1)
		*(ft_strchr(dest, '\0') - 1) = '\0';
	_process_dot(dest);
	_process_dotdot(dest);
	return (MS_SUCCESS);
}

int	ms_cd(char **args, t_cmd_params *params, ...)
{
	char	path[PATH_MAX + 1];
	int		ret;

	if (_validate_input(args, params))
		return (MS_CMD_ERROR_OK);
	ret = _get_path(path, args[1]);
	if (ret)
	{
		params->wstatus = _set_wstatus(ret, 0);
		return (MS_CMD_ERROR_OK);
	}
	if (chdir(path))
	{
		ft_print_err(strerror(errno), 3, "minishell", "cd", args[1]);
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
	}
	else
		params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	ms_setenv("OLDPWD", ms_getenv("PWD"));
	ms_setenv("PWD", path);
	return (MS_CMD_ERROR_OK);
}
