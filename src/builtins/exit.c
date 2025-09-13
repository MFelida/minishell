/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 17:25:15 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define LLONG_MAX_S "9223372036854775807"
#define LLONG_MIN_S "-9223372036854775808"

static bool	_is_numeric(const char *arg)
{
	int		is_neg;
	size_t	i;

	is_neg = 0;
	i = 0;
	if (*arg == '-')
	{
		arg++;
		is_neg = 1;
	}
	while (*arg == '0')
		arg++;
	while (arg[i])
	{
		if (((!is_neg && i >= sizeof(LLONG_MAX_S))
				|| (is_neg && i >= sizeof(LLONG_MIN_S)))
			|| !ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return ((!is_neg && ft_strncmp(arg, LLONG_MAX_S, sizeof(LLONG_MAX_S)) <= 0)
		|| (is_neg && ft_strncmp(arg, &LLONG_MIN_S[1], sizeof(LLONG_MIN_S)) <= 0));
}

int	ms_exit(const char **args, t_cmd_params *params, ...)
{
	const char	*exit_str;

	if (args[1] == NULL)
		params->wstatus = _set_wstatus(ft_atoi(ms_getenv("?")), 0);
	else if (!_is_numeric(args[1]))
	{
		ft_print_err("numeric argument required", 3, "minishell", "exit", args[1]);
		params->wstatus = _set_wstatus(MS_BUILTIN_MISUSE, 0);
	}
	else if (args[2])
	{
		ft_print_err("too many arguments", 2, "minishell", "exit");
		params->wstatus = _set_wstatus(MS_FAILURE, 0);
		return (MS_CMD_ERROR_OK);
	}
	exit_str = args[1];
	if (ft_strlen(exit_str) > 8)
		exit_str += ft_strlen(exit_str) - 8;
	params->wstatus = _set_wstatus(ft_atoi(exit_str), 0);
	if (params->context & MS_CMD_CONTEXT_PIPE)
		return (MS_CMD_ERROR_OK);
	return (MS_CMD_ERROR_SHOULD_EXIT);
}
