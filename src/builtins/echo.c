/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:21:21 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 15:56:44 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include "stdbool.h"
#include <stddef.h>
#include <unistd.h>

int	ms_echo(char **args, t_cmd_params *params, ...)
{
	size_t	i;
	size_t	first;
	bool	nl;
	int		write_error;

	i = 0;
	nl = true;
	write_error = MS_SUCCESS;
	while (args[++i] && _is_option(args[i]) && !_invalid_option(args[i], "n"))
			nl = !ft_strchr(args[i], 'n');
	first = i;
	while (args[i])
	{
		if (i > first && ft_putchar_fd(' ', STDOUT_FILENO) < 0)
			write_error = MS_FAILURE;
		if (ft_putstr_fd(args[i], 1))
			write_error = MS_FAILURE;
		i++;
	}
	if (nl && ft_putchar_fd('\n', STDOUT_FILENO) < 0)
		write_error = MS_FAILURE;
	params->wstatus = _set_wstatus(write_error, 0);
	return (MS_CMD_ERROR_OK);
}
