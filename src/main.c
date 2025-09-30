/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 23:31:19 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "execute.h"
#include "parse_tree.h"
#include "parser.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

#ifndef DEBUG
void	init_minishell(void)
{
	init_env();
	ms_setenv("PS1", "minishell$ ");
	ms_setenv("?", "0");
}
#else
void	init_minishell(void)
{
	printf("%d\n", getpid());
	init_env();
	ms_setenv("PS1", "minishell$ ");
	ms_setenv("?", "0");
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
		if (!ft_strncmp("exit", input, ft_strlen("exit") + 1))
		{
			free(input);
			break ;
		}
		add_history(input);
		pt = get_parse_tree(input);
		if (!pt)
			continue ;
		ret = exec_parsetree(pt);
		free_parse_tree(&pt);
	}
	printf("exit\n");
	ft_exit(ft_atoi(ms_getenv("?")));
}
