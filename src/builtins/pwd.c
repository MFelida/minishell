/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 15:35:38 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int	valid_pwd(const char *env_pwd)
{
	char		buff[PATH_MAX + 1];
	struct stat	env_stat;
	struct stat	cwd_stat;

	if (!getcwd(buff, PATH_MAX))
	{
		ft_print_err(strerror(errno), 2, "minishell", "pwd");
		return (0);
	}
	if (stat(buff, &cwd_stat))
	{
		ft_print_err(strerror(errno), 3, "minishell", "pwd", buff);
		return (0);
	}
	if (stat(env_pwd, &env_stat))
		return (0);
	return (env_stat.st_ino == cwd_stat.st_ino);
}

// TODO: Don't use getcwd, print from $PWD to respect logical path (symbolic links)
int	ms_pwd(const char **args, t_cmd_params *params, ...)
{
	char		buff[PATH_MAX + 1];
	const char	*pwd;
	size_t		i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3, "minishell", "pwd", _invalid_option(args[i], "p"));
			params->wstatus = _set_wstatus(MS_BUILTIN_MISUSE, 0);
			return (MS_CMD_ERROR_OK);
		}
	}
	pwd = buff;
	if (ms_getenv("PWD") && valid_pwd(ms_getenv("PWD")))
		pwd = ms_getenv("PWD");
	else if (!getcwd(buff, PATH_MAX))
	{
		ft_print_err(strerror(errno), 2, "minishell", "pwd");
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
		return (MS_CMD_ERROR_FAILURE);
	}
	ft_printf("%s\n", pwd);
	params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	return (MS_CMD_ERROR_OK);
}
