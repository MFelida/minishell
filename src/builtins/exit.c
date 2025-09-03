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
#include "exit_statuses.h"
#include "libft.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

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
		i = 1;
		is_neg = 1;
	}
	while (arg[i])
	{
		if (((!is_neg && i >= sizeof(LLONG_MAX_S)) || (is_neg && i >= sizeof(LLONG_MIN_S))) || !ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return ((!is_neg && ft_strncmp(arg, LLONG_MAX_S, sizeof(LLONG_MAX_S)))
		|| (is_neg && ft_strncmp(arg, LLONG_MIN_S, sizeof(LLONG_MIN_S))));
}

int	ms_exit(const char **args)
{
	if (!args[1])
		return (MS_SUCCESS);
	if (!_is_numeric(args[1]))
	{
		ft_print_err("numeric argument required", 3, "minishell", "exit", args[1]);
		return (MS_BUILTIN_MISUSE);
	}
	if (args[2])
	{
		ft_print_err("too many arguments", 2, "minishell", "exit");
		return (MS_FAILURE);
	}
	return (MS_SUCCESS);
}
