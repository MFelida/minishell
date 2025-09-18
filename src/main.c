/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/18 12:41:30 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "parsing_header.h"
#include <unistd.h>

int	main(void)
{
	char	*input;
	t_parsing_context	*par_con;

	par_con = (t_parsing_context *)malloc(sizeof(t_parsing_context) * 1);
	if (!par_con)
		exit (1); //or equivalent exit func, need to return exit state for $?
	
	init_parcon(par_con);
	//still gotta handle signals
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			first_pass(input, par_con);
			printf("Syntax Tree:\n");
			print_ast(par_con->root, 0);
		}
		printf("your command: %s\n", input);
		free(input);
	}
	printf("Hello, World! BRUH\n");
	return (EXIT_SUCCESS);
}

// heredoc, account for cat << "eof", this does not expand variables, if it is just cat << eof, it expands the variable like $path for example
// 'hello'bye"$USER"gfcjtvg
// hello'bye' //hello gets concatenated to bye, if you use hello'bye' as a heredoc delimiter, it also won't expand variables
