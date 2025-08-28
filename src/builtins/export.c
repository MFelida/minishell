// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   export.c                                           :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:23:40 by mifelida     #+#    #+#                  //
/*   Updated: 2025/08/28 16:41:54 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#include "env.h"
#include "exit_statuses.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

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
	size_t	len;

	value = ft_strchr(keyvalpair, '=') + 1;
	if (value <= (char *) 1)
	{
		value = "";
		len = ft_strlen(keyvalpair);
	}
	else
		len = value	- keyvalpair - 1;
	key = ft_substr(keyvalpair, 0, len);
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
		{
			ft_print_err("export doesn't support any options", 3, "minishell", "export", args[i]);
			return (MS_BUILTIN_MISUSE);
		}
		if (!_is_valid_key(args[i]))
		{
			ft_print_err("not a valid identifier", 3, "minishell", "export", args[i]);
			return (MS_FAILURE);
		}
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
