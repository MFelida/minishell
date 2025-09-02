/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/02 18:40:41 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "command/utils.h"
#include "env.h"
#include "fake_parser.h"
#include "libft.h"
#include "redirect.h"

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
	int				res;

	init_minishell();
	if (argc <= 1)
	{
		ft_exit(EXIT_FAILURE);
	}
	pt = get_parse_tree(argv[1]);
	params = cmd_params_default();
	param_list = NULL;
	params.head = &param_list;
	cmd_next_node(&params, pt);
	last_cmd = *(t_cmd_params *) ft_lstlast((t_list *) param_list);
	waitpid(last_cmd.pid, &last_cmd.wstatus, 0);
	waitpid(-1, NULL, 0);
	res = WEXITSTATUS(last_cmd.wstatus);
	free_cmd_params(params);
	ft_exit(res);
}
