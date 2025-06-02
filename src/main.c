/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/02 17:02:18 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void	signal_handler(int	signal)
{
	(void)signal;
	rl_on_new_line();
	rl_replace_line("\n", 1);
	rl_redisplay();
}

int	main(void)
{
	char	*input;

	signal(SIGINT, signal_handler);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf ("exit\n");
			break;
		}
		if (*input)
		{
			add_history(input);
			//parser_sender(input);
		}
		printf("your command: %s\n", input);
		free(input);
	}
	printf("Hello, World! BRUH\n");
	return (EXIT_SUCCESS);
}
