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
		getcwd(path, PATH_MAX + 1);
		ft_strlcat(path, "/", PATH_MAX + 1);
		ft_strlcat(path, args[1], PATH_MAX + 1);
	}
	if (chdir(path))
	{
		ft_print_err(strerror(errno), 2, "minishell", "cd");
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
	}
	else
		params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	ms_setenv("PWD", getcwd(path, PATH_MAX + 1));
	// TODO: DELETE
	printf("%s\n", path);
	return (MS_CMD_ERROR_OK);
}
