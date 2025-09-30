// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   export.c                                           :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:23:40 by mifelida     #+#    #+#                  //
/*   Updated: 2025/09/03 18:45:26 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static int	_is_valid_key(const char *key)
{
	if (!*key || !(ft_isalpha(*key) || *key == '_'))
		return (0);
	while (*++key && *key != '=')
		if (!(ft_isalnum(*key) || *key == '_'))
			return (key[0] == '+' && key[1] == '=');
	return (1);
}

static int	_export_setenv(const char *keyvalpair)
{
	char	*key;
	char	*value;
	size_t	len;
	int		append;

	value = ft_strchr(keyvalpair, '=') + 1;
	if (value <= (char *) 1)
		return (ms_setenv(keyvalpair, ""));
	append = ft_strnstr(keyvalpair, "+=", ft_strlen(keyvalpair)) == ft_strchr(keyvalpair, '=') - 1;
	len = value	- keyvalpair - 1 - append;
	key = ft_substr(keyvalpair, 0, len);
	if (!key)
		return (1);
	if (append && ms_getenv(key))
	{
		value = ft_strjoin(ms_getenv(key), value);
		ms_setenv(key, value);
		free(value);
	}
	else
		ms_setenv(key, value);
	free(key);
	return (0);
}

static int	_validate_args(char **args, t_cmd_params *params)
{
	size_t	i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3, "minishell", "export", _invalid_option(args[i], ""));
			params->wstatus = _set_wstatus(MS_BUILTIN_MISUSE, 0);
			return (1);
		}
		else if (!_is_valid_key(args[i]))
		{
			ft_print_err("not a valid identifier", 3, "minishell", "export", args[i]);
			params->wstatus = _set_wstatus(MS_FAILURE, 0);
			return (1);
		}
	}
	return (0);
}

int	ms_export(char **args, t_cmd_params *params, ...)
{
	size_t	i;

	if (!args[1])
		print_env(MS_ENV_PRINT_SORTED, MS_ENV_PRINT_INCL_EMPTY);
	if (_validate_args(args, params))
		return (MS_CMD_ERROR_OK);
	i = 1;
	while (args[i])
	{
		if (_export_setenv(args[i++]))
		{
			params->wstatus = _set_wstatus(MS_FAILURE, 0);
			return (MS_CMD_ERROR_FAILURE);
		}
	}
	params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	return (MS_CMD_ERROR_OK);
}
