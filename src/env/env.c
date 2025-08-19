// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   env.c                                             :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 10:42:11 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/19 13:24:50 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "hashmap.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>

static t_hm	env_hm = {0};

void	free_env(void)
{
	free_hm(&env_hm);
}

int	init_env(void)
{
	extern char	**environ;
	size_t		i;
	char		*key;
	char		*value;

	i = 0;
	while (environ[i])
	{
		value = ft_strchr(environ[i], '=') + 1;
		key = ft_substr(environ[i], 0, value - environ[i] - 1);
		if (hm_set_value(&env_hm, key, value))
		{
			free(key);
			free_env();
			return (1);
		}
		free(key);
		i++;
	}
	return (0);
}

const char	*ms_getenv(const char *key)
{
	return (hm_get_value(&env_hm, key));
}

int	ms_setenv(const char *key, const char *value)
{
	return (hm_set_value(&env_hm, key, value));
}

int	ms_unset(const char *key)
{
	return (hm_unset(&env_hm, key));
}
