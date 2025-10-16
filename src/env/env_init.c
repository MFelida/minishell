/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:30:35 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 13:33:51 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

static void	_init_pwd(void)
{
	char	*pwd;

	if (ms_getenv("PWD")
		&& valid_pwd(ms_getenv("PWD")))
		return ;
	pwd = getcwd(NULL, 0);
	ms_setenv("PWD", pwd);
	free(pwd);
}

int	init_env(void)
{
	extern char	**environ;
	size_t		i;
	char		*key;
	char		*value;

	i = ((size_t) 0) - 1;
	ft_atexit(free_env);
	while (environ[++i])
	{
		value = ft_strchr(environ[i], '=') + 1;
		if (value <= (char *) 1)
			value = "";
		key = ft_substr(environ[i], 0, value - environ[i] - 1);
		if (!key)
			return (1);
		if (ms_setenv(key, value))
		{
			free(key);
			return (1);
		}
		free(key);
	}
	_init_pwd();
	ms_setenv("?", "0");
	return (0);
}
