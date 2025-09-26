/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:17:03 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/23 12:34:06 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "redirect.h"
#include "tokens.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static int	_ms_waitpid(int *ret, t_cmd_params *params, int options)
{
	*ret = waitpid(params->pid, &params->wstatus, options);
	return (*ret);
}

int	exec_parsetree(t_parse_tree	*pt)
{
	t_cmd_params	params;
	t_cmd_params	last_cmd;
	char			*exit_status;
	int				wait_ret;
	int				ret;

	params = cmd_params_default();
	exit_status = NULL;
	ret = cmd_next_node(&params, pt);
	close_fds();
	if (ret & MS_CMD_ERROR_FAILURE)
	{
		ms_setenv("?", "1");
		free_cmd_params(params);
		return (ret);
	}
	last_cmd = *(t_cmd_params *) ft_lstlast((t_list *) *params.head);
	if (last_cmd.context & MS_CMD_CONTEXT_BLTIN)
		exit_status = ft_itoa(WEXITSTATUS(last_cmd.wstatus));
	else
	{
		while (_ms_waitpid(&wait_ret, &last_cmd, WNOHANG) == 0)
			; // TODO: signal stuff
		if (wait_ret < 0)
			ft_print_err(strerror(errno), 2, "minishell", "waitpid");
		else if (WIFSIGNALED(last_cmd.wstatus))
			exit_status = ft_itoa(MS_SIGNAL_EXIT + WTERMSIG(last_cmd.wstatus));
		else
			exit_status = ft_itoa(WEXITSTATUS(last_cmd.wstatus));
	}
	if (exit_status)
	{
		ms_setenv("?", exit_status);
		free(exit_status);
	}
	else
		ms_setenv("?", "1");
	while (waitpid(-1 , NULL, WNOHANG) >= 0)
		; // TODO: SIGNAL STUFF
	free_cmd_params(params);
	return (ret);
}
