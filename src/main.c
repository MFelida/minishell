/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/04 18:36:23 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "parsing_header.h"

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
			printf("exit\n");
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

int	parser_processer(char *input)
{
	int		i;
	int		apo_bool;
	char	*seperated_input;

	apo_bool = 0;
	i = 0;
	while(input[i] != '\0')
	{
		while (ft_isspace(input[i]) && apo_bool != 1)
			i++;
		while (!ft_isspace(input[i]) || apo_bool == 1)
		{
			if (input[i] == '"')
			{
				if (apo_bool == 1)
					apo_bool = 0;
				else
					apo_bool = 1;
				i++;
			}
			// basically strcpy but with comma ',' seperators between each token, then pass it to split with , as the c
			// then you will have a malloced 2d array with each token and can look at them independently
			// might be the move
		}
	}
}

int	within_apo(char *input, int i)
{
	while(input[i] != '"')
}

int	parser_sender(char *input)
{
	
}