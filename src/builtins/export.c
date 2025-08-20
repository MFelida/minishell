// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   export.c                                          :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:23:40 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 17:58:33 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "env.h"
#include "exit_statuses.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>

static int	_is_valid_key(const char *key)
{
	if (!(ft_isalpha(*key) || *key == '_'))
		return (0);
	while (*++key && *key != '=')
		if (!(ft_isalnum(*key) || *key == '_'))
			return (0);
	return (1);
}

static int	_is_option(const char *arg)
{
	return (*arg == '-');
}

static int	_export_setenv(const char *keyvalpair)
{
	char	*key;
	char	*value;

	if (ft_strchr(keyvalpair, '='))
		value = ft_strchr(keyvalpair, '=') + 1;
	else
		value = "";
	key = ft_substr(keyvalpair, 0, ft_strchr(keyvalpair, '=') - keyvalpair);
	if (!key)
		return (1);
	ms_setenv(key, value);
	free(key);
	return (0);
}

int	ms_export(char **args)
{
	size_t	i;

	if (!args[1])
		print_env(1, 1);
	i = 0;
	while (args[i])
	{
		if (_is_option(args[i]))
			return (MS_BUILTIN_MISUSE);
		//	TODO: Print error messages
		if (!_is_valid_key(args[i]))
			return (MS_FAILURE);
		i++;
	}
	i = 0;
	while (args[i])
	{
		if (_export_setenv(args[i]))
			return (MS_FAILURE);
		i++;
	}
	return (MS_SUCCES);
}
