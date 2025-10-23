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
#include <stdlib.h>

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
	else if (*arg == '+')
		arg++;
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
		|| (is_neg
			&& ft_strncmp(arg, &LLONG_MIN_S[1], sizeof(LLONG_MIN_S)) <= 0));
}

static int	_get_status(const char *status)
{
	const char	*_status;
	int			res;

	_status = status;
	if (status[0] == '-')
		_status++;
	if (ft_strlen(_status) > 8)
		_status += ft_strlen(_status) - 8;
	res = ft_atoi(_status);
	if (status[0] == '-' && res > 0)
		res *= -1;
	return (res);
}

int	ms_exit(const char **args, t_cmd_params *params, ...)
{
	int	exit_status;
	int	ret_val;

	exit_status = MS_BUILTIN_MISUSE;
	ret_val = MS_CMD_ERROR_SHOULD_EXIT;
	if (params->context == MS_CMD_CONTEXT_PIPE)
		ret_val = MS_CMD_ERROR_OK;
	if (args[1] == NULL)
		exit_status = ft_atoi(ms_getenv("?"));
	else if (!_is_numeric(args[1]))
		ft_print_err("numeric argument required", 3,
			"minishell", "exit", args[1]);
	else if (args[2])
	{
		ft_print_err("too many arguments", 2, "minishell", "exit");
		exit_status = MS_FAILURE;
		ret_val = MS_CMD_ERROR_OK;
	}
	else
		exit_status = _get_status(args[1]);
	params->wstatus = _set_wstatus(exit_status, 0);
	return (ret_val);
}
