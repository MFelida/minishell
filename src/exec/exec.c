/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:17:03 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 23:31:00 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "exit_statuses.h"
#include "libft.h"
#include "redirect.h"
#include "parse_tree.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static int	_ms_waitpid(int *ret, t_cmd_params *params, int options)
{
	if (!params)
		*ret = waitpid(-1, NULL, options);
	else
		*ret = waitpid(params->pid, &params->wstatus, options);
	return (*ret);
}

int	exec_parsetree(t_parse_node	*pt)
{
	t_cmd_params	params;
	t_cmd_params	last_cmd;
	int				wait_ret;
	int				ret;

	params = cmd_params_default();
	params.pt = pt;
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
		ms_set_exitstatus(WEXITSTATUS(last_cmd.wstatus));
	else
	{
		while (_ms_waitpid(&wait_ret, &last_cmd, WNOHANG) == 0)
			; // TODO: signal stuff
		if (wait_ret < 0)
			ft_print_err(strerror(errno), 2, "minishell", "waitpid");
		else if (WIFSIGNALED(last_cmd.wstatus))
			ms_set_exitstatus(MS_SIGNAL_EXIT + WTERMSIG(last_cmd.wstatus));
		else
			ms_set_exitstatus(WEXITSTATUS(last_cmd.wstatus));
	}
	while (_ms_waitpid(&wait_ret, NULL, WNOHANG) >= 0)
		; // TODO: SIGNAL STUFF
	free_cmd_params(params);
	return (ret);
}
