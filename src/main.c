/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/16 12:07:29 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "parsing_header.h"
#include <unistd.h>

void	signal_handler(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(void) // https://www.cs.sjsu.edu/faculty/pearce/java1/streams/tokens.html
{
	char	*input;
	t_token_list	*head;

	head = NULL;
	// tokenize input (think operators and identifiers, whitespace)
	// check for commands, check if operators are control, otherwise split metachars
	// set up tree and send it over
	// tokenize, put in struct (pointer next (FIRST), token, string), then organize for right-
	// order in tree

	rl_catch_signals = 0;
	signal(SIGINT, signal_handler);
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
			// parser_processor(input);
		}
		printf("your command: %s\n", input);
		free(input);
	}
	printf("Hello, World! BRUH\n");
	return (EXIT_SUCCESS);
}

size_t	whitespace_len(char *string)
{
	size_t	i;

	i = 0;
	while (string[i] != '\0' && ft_isspace(string[i]))
		i++;
	return (i);
}

int	parser_processor(char *input)
{
	int	i;
	int	j;
	size_t	len;
	char *new_input; // replace it with struct later, initialize struct in main

	j = 0;
	i = 0;
	while(isspace(input[i]))
	i++;
	if (input[i] == '"')
	{
		len = whitespace_len(input[++i]);
		new_input = (char *)malloc(len);
		while(input[++i] != '"')
		{
			new_input[j] = input[i];
		}
		new_input = '\0';
	}
}

int	pipe_finder(char *input)
{
	int	i;
	int	pipecount;

	i = 0;
	pipecount = 0;
	while(input[i])
	{
		if (input[i] == "|")
			pipecount++;
		i++;
	}
	return (pipecount);
}

void	add_node(char *string, t_token_list *head)
{
	if (!head)
		head = mk_node(string, head);
	else
		head->next = mk_node(string, head);
}

t_token_list	*mk_node(char *string, t_token_list *head)
{
	t_token_list *node;

	node = (t_token_list *)malloc(sizeof(t_token_list));
	node->string = string;
	node->type = typefinder(string);
	node->next = NULL;
	return (node);
}

int	typefinder(char *string)
{
	
}

// int	parser_processer(char *input)
// {
// 	int		i;
// 	int		apo_bool;
// 	char	*seperated_input;

// 	apo_bool = 0;
// 	i = 0;
// 	while(input[i] != '\0')
// 	{
// 		while (ft_isspace(input[i]) && apo_bool != 1)
// 			i++;
// 		while (!ft_isspace(input[i]) || apo_bool == 1)
// 		{
// 			if (input[i] == '"')
// 			{
// 				if (apo_bool == 1)
// 					apo_bool = 0;
// 				else
// 					apo_bool = 1;
// 				i++;
// 			}
// 			// basically strcpy but with comma ',' seperators between each token, then pass it to split with , as the c
// 			// then you will have a malloced 2d array with each token and can look at them independently
// 			// might be the move
// 			// think ft_split for argument intake as well
// 		}
// 	}
// }

// int	within_apo(char *input, int i)
// {
// 	while(input[i] != '"')
// }

// int	parser_sender(char *input)
// {
	
// }