/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 17:39:34 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "fake_parser.h"
#include "libft.h"
#include "redirect.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int	main(int argc, char *argv[])
{
	t_parse_node	*pt;
	t_cmd_params	params;
	t_cmd_params	*param_list;
	t_cmd_params	last_cmd;

	if (argc <= 1)
	{
		printf("Hello, World!\n");
		return (EXIT_SUCCESS);
	}
	pt = get_parse_tree(argv[1]);
	params = cmd_params_default();
	param_list = NULL;
	params.head = &param_list;
	cmd_next_node(&params, pt);
	close_fds(params.open_fds);
	last_cmd = *(t_cmd_params *) ft_lstlast((t_list *) param_list);
	waitpid(last_cmd.pid, &last_cmd.wstatus, 0);
	waitpid(-1, NULL, 0);
	return (WEXITSTATUS(last_cmd.wstatus));
}
