/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/02 18:25:46 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "libft.h"
#include "utils.h"

#include <errno.h>
#include <linux/limits.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

int	ms_pwd(const char **args)
{
	char	buff[PATH_MAX + 1];
	size_t	i;

	i = 1;
	while (args[i])
	{
		if (_is_option(args[i]))
		{
			ft_print_err("pwd doesn't support any options", 3, "minishell", "pwd", args[i]);
			return (MS_BUILTIN_MISUSE);
		}
		i++;
	}
	if (!getcwd(buff, PATH_MAX))
	{
		ft_print_err(strerror(errno), 2, "minishell", "pwd");
		return (MS_FAILURE);
	}
	ft_printf("%s\n", buff);
	return (MS_SUCCESS);
}
