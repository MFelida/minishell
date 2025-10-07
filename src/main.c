/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/30 13:44:08 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "execute.h"
#include "exit_statuses.h"
#include "parse_tree.h"
#include "parser.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

sig_atomic_t	g_signal = 0;

#ifndef DEBUG
void	init_minishell(void)
{
	init_env();
	if (ms_is_interactive())
		ms_setenv("PS1", "minishell$ ");
	ms_set_exitstatus(0);
	setup_sighandlers();
}
#else
void	init_minishell(void)
{
	printf("%d\n", getpid());
	init_env();
	if (ms_is_interactive())
		ms_setenv("PS1", "minishell$ ");
	ms_set_exitstatus(0);
	setup_sighandlers();
}
#endif

int	main(void)
{
	int				ret;
	char			*input;
	t_parse_node	*pt;

	init_minishell();
	ret = 0;
	while (!(ret & MS_CMD_ERROR_SHOULD_EXIT))
	{
		input = readline(ms_getenv("PS1"));
		if (!input)
			break ;
		if (!*input)
			continue ;
		add_history(input);
		pt = get_parse_tree(input);
		if (pt)
			ret = exec_parsetree(pt);
		else
			ms_set_exitstatus(MS_BUILTIN_MISUSE);
		free_parse_tree(&pt);
	}
	printf("exit\n");
	ft_exit(ft_atoi(ms_getenv("?")));
}
