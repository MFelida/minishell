/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/23 12:39:39 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "execute.h"
#include "exit_statuses.h"
#include "fake_parser.h"
#include "libft.h"
#include "redirect.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
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

	init_minishell();
	if (argc <= 1)
		ft_exit(EXIT_FAILURE);
	pt = get_parse_tree(argv[1]);
	exec_parsetree(pt);
	ft_exit(ft_atoi(ms_getenv("?")));
}
