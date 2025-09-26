/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/24 13:37:18 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "env.h"
#include "execute.h"
#include "libft.h"
#include "parsing_header.h"

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
}
#else
void	init_minishell(void)
{
	printf("%d\n", getpid());
	init_env();
}
#endif

int	main(void)
{
	t_parsing_context	*par_con;
	int					ret;

	par_con = (t_parsing_context *)malloc(sizeof(t_parsing_context) * 1);
	if (!par_con)
		exit (1); //or equivalent exit func, need to return exit state for $?
	init_minishell();
	while (1)
	{
		init_parcon(par_con);
		par_con->arg = readline("minishell> ");
		if (!par_con->arg)
		{
			printf("exit\n");
			break ;
		}
		if (*par_con->arg)
		{
			add_history(par_con->arg);
			first_pass(par_con);
			ret = exec_parsetree(par_con->root);
			//printf("Syntax Tree:\n");
			//print_ast(par_con->root, 0);
		}
		free(par_con);
		//printf("your command: %s\n", input);
		// free(input);
		if (ret & MS_CMD_ERROR_SHOULD_EXIT)
			break;
	}
	// printf("Hello, World! BRUH\n");
	return (ft_atoi(ms_getenv("?")));
}

// heredoc, account for cat << "eof", this does not expand variables, if it is just cat << eof, it expands the variable like $path for example
// 'hello'bye"$USER"gfcjtvg
// hello'bye' //hello gets concatenated to bye, if you use hello'bye' as a heredoc delimiter, it also won't expand variables
