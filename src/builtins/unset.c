/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 15:36:01 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>

int	ms_unset(const char **args, t_cmd_params *params, ...)
{
	size_t	i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3, "minishell", "unset", _invalid_option(args[i], ""));
			params->wstatus = _set_wstatus(MS_BUILTIN_MISUSE, 0);
			return (MS_CMD_ERROR_OK);
		}
	}
	i = 0;
	while (args[++i])
	{
		ms_unsetenv(args[i]);
	}
	params->wstatus = _set_wstatus(MS_SUCCESS, 0);
	return (MS_CMD_ERROR_OK);
}
