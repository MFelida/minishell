/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/03 19:05:07 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "command/utils.h"
#include "env.h"
#include "exit_statuses.h"
#include "fake_parser.h"
#include "libft.h"
#include "redirect.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void	init_minishell(void)
{
	init_env();
	ft_atexit(close_fds);
	// TODO: Setup signal handler
}

int	main(int argc, char *argv[])
{
	t_parse_node	*pt;
	t_cmd_params	params;
	t_cmd_params	*param_list;
	t_cmd_params	last_cmd;
	char			*exit_status;

	init_minishell();
	if (argc <= 1)
	{
		ft_exit(EXIT_FAILURE);
	}
	pt = get_parse_tree(argv[1]);
	params = cmd_params_default();
	param_list = NULL;
	params.head = &param_list;
	if (!(cmd_next_node(&params, pt) & MS_CMD_ERROR_SHOULD_EXIT))
	{
		last_cmd = *(t_cmd_params *) ft_lstlast((t_list *) param_list);
		waitpid(last_cmd.pid, &last_cmd.wstatus, 0);
		if (WIFSIGNALED(last_cmd.wstatus))
			ms_setenv("?", exit_status = ft_itoa(MS_SIGNAL_EXIT + WTERMSIG(last_cmd.wstatus)));
		else
			ms_setenv("?", exit_status = ft_itoa(WEXITSTATUS(last_cmd.wstatus)));
		free(exit_status);
		waitpid(-1, NULL, 0);
	}
	free_cmd_params(params);
	ft_exit(ft_atoi(ms_getenv("?")));
}
