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

#include "env.h"
#include "execute.h"
#include "parsing_header.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

int	init_minishell(void)
{
	init_env();
}

int	main(void)
{
	t_parsing_context	*par_con;

	par_con = (t_parsing_context *)malloc(sizeof(t_parsing_context) * 1);
	if (!par_con)
		exit (1); //or equivalent exit func, need to return exit state for $?
	
	init_parcon(par_con);
	init_env();
	//still gotta handle signals
	while (1)
	{
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
			exec_parsetree(par_con->root);
			//printf("Syntax Tree:\n");
			//print_ast(par_con->root, 0);
		}
		//printf("your command: %s\n", input);
		// free(input);
	}
	printf("Hello, World! BRUH\n");
	return (EXIT_SUCCESS);
}

// heredoc, account for cat << "eof", this does not expand variables, if it is just cat << eof, it expands the variable like $path for example
// 'hello'bye"$USER"gfcjtvg
// hello'bye' //hello gets concatenated to bye, if you use hello'bye' as a heredoc delimiter, it also won't expand variables
