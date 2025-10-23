/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_child_proc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:40:29 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 13:49:57 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "execute.h"
#include "exit_statuses.h"
#include "libft.h"
#include "redirect.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

_Noreturn static void	_clean_and_exit(int exit_status, t_cmd_params params)
{
	close_fds();
	free_cmd_params(params);
	ft_exit(exit_status);
}

void	_cmd_exec_bin_err(t_cmd_params *params, int find_bin_ret)
{
	struct stat	stat_buff;

	if (!find_bin_ret)
		return ;
	if (find_bin_ret == MS_CMD_NOT_FOUND
		&& is_rel_path(params->cmd_args[0]) > 1)
		ft_print_err("No such file or directory",
			2, "minishell", params->cmd_args[0]);
	else if (find_bin_ret == MS_CMD_NOT_FOUND)
		ft_print_err("command not found", 2, "minishell", params->cmd_args[0]);
	else if (find_bin_ret == MS_PERM_DENIED)
	{
		if (stat(params->bin_path, &stat_buff) == 0
			&& S_ISDIR(stat_buff.st_mode))
			ft_print_err("Is a directory",
				2, "minishell", params->cmd_args[0]);
		else
			ft_print_err("Permission denied",
				2, "minishell", params->cmd_args[0]);
	}
	else
		ft_print_err(strerror(errno), 2, "minishell", params->cmd_args[0]);
	_clean_and_exit(find_bin_ret, *params);
}

_Noreturn void	cmd_exec(t_cmd_params params)
{
	int			find_bin_ret;

	if (do_redirs(&params))
		_clean_and_exit(MS_FAILURE, params);
	close_fds();
	if (*params.cmd_args)
	{
		find_bin_ret = find_bin(params.bin_path, params.cmd_args[0]);
		_cmd_exec_bin_err(&params, find_bin_ret);
		params.envp = ms_getenv_full(0, 1, 1);
		reset_signal();
		execve(params.bin_path, params.cmd_args, params.envp);
		ft_print_err(strerror(errno), 2, "minishell", params.cmd_args[0]);
		_clean_and_exit(MS_PERM_DENIED, params);
	}
	else
		_clean_and_exit(MS_SUCCESS, params);
}
