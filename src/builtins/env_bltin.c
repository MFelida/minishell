/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bltin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:34:14 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 16:51:04 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>

int	ms_env(const char **args)
{
	size_t	i;

	i = 0;
	while (args[++i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("invalid option", 3, "minishell", "env", _invalid_option(args[i], ""));
			return (MS_BUILTIN_MISUSE);
		}
	}
	print_env(MS_ENV_PRINT_UNSORTED, MS_ENV_PRINT_EXCL_EMPTY);
	return (MS_SUCCESS);
}
