/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/07/21 10:57:48 by amel-fou         ###   ########.fr       */
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
	free_context_struct(par_con); // figure out how to do this cuz it doesn't work
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//fukkit, make token counter to allocate token struct, no linked lists, then make an identification function for each token, which also then creates a AST. I think that's the move.'

int	main(void) // https://www.cs.sjsu.edu/faculty/pearce/java1/streams/tokens.html
{
	char	*input;
	t_token_list		*head;
	t_parsing_context	*par_con;

	par_con = (t_parsing_context *)malloc(sizeof(t_parsing_context) * 1);
	if (!par_con)
		exit (1); //or equivalent exit func, need to return exit state for $?
	
	init_parcon(par_con, head);
	head = NULL;
	// tokenize input (think operators and identifiers, whitespace)
	// check for commands, check if operators are control, otherwise split metachars
	// set up tree and send it over
	// tokenize, put in struct (pointer next (FIRST), token, string), then organize for right-
	// order in tree

	rl_catch_signals = 0;
	signal(SIGINT, signal_handler);
	init_parcon;
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
			// neo_parser_processor(input, head);
		}
		printf("your command: %s\n", input);
		free(input);
	}
	printf("Hello, World! BRUH\n");
	return (EXIT_SUCCESS);
}

heredoc, account for cat << "eof", this does not expand variables, if it is just cat << eof, it expands the variable like $path for example
'hello'bye"$USER"
hello'bye' //hello gets concatenated to bye, if you use hello'bye' as a heredoc delimiter, it also won't expand variables

int	neo_parser_processor_v2(char *input, t_token_list **head)
{
	char	*curr; //so revamp these bools into one single int using the enums for clarity.
	size_t	word_pos; //then, turn this into a state machine, if in state of quotes, head into a seperate whileloop, return the count we've incremented and go back to the initial whileloop
	size_t	i;
	int		quote_status;// revamped "bool" here
	int		temp;

	temp = 0;
	quote_status = NOT_IN_QUOTES; // see enums in header
	word_pos = 0;
	while(input[i] != '\0')
	{
		if (quote_status = ESCAPING)
		{
			curr[word_pos++] = input[i++];
			if (quote_status = SINGLE_ESCAPING)
				quote_status = IN_SINGLE_QUOTE;
			else if (quote_status = DOUBLE_ESCAPING)
				quote_status = IN_DOUBLE_QUOTE;
			else
				quote_status = NOT_IN_QUOTES;
			continue;
		}
		else if (input[i] == "\\")
		{
			if (quote_status = IN_SINGLE_QUOTE)
				quote_status = SINGLE_ESCAPING;
			else if (quote_status = IN_DOUBLE_QUOTE)
				quote_status = DOUBLE_ESCAPING;
			else
				quote_status = ESCAPING;
			i++;
			continue;
		}
		else if (input[i] == '\'' && quote_status != IN_DOUBLE_QUOTE)
		{
			quote_status = IN_SINGLE_QUOTE;
			temp = single_qoute_lex(input, head, curr);
			if (temp < 0)
			{
				i = 0;
				return (0); // don't know if I need to return error if it fails here, I think just return 0, unless something actually program breaking happens like a malloc fail
			}
			else
				i += temp;
			continue;
		}
		else if (input[i] == '"' && quote_status != IN_SINGLE_QUOTE)
		{
			quote_status = IN_DOUBLE_QUOTE;
			temp = single_qoute_lex(input, head, curr);
			if (temp < 0)
			{
				i = 0;
				return (0); // don't know if I need to return error if it fails here, I think just return 0, unless something actually program breaking happens like a malloc fail
			}
			else
				i += temp;
			continue;
		}
		else if (input[i] == '|' && quote_status != IN_DOUBLE_QUOTE && quote_status != IN_SINGLE_QUOTE)
		{
			add_node(curr, head);
			ft_bzero(curr, word_pos + 1); //probably make a wrapper that bzero's to begin with
			word_pos = 0;
			add_node("|", head);
			while (isspace(input[i]))
				i++;
			continue;
		}
		if (isspace(input[i]) && quote_status != IN_DOUBLE_QUOTE && quote_status != IN_SINGLE_QUOTE) //replace with ismetachar, define whether it's whitespace or metachar that needs to be tokenized, generate node accordingly
		{
			if (curr)
			{
				add_node(curr, head);
				ft_bzero(curr, word_pos + 1); //probably make a wrapper that bzero's to begin with
				word_pos = 0;
			}
			while (isspace(input[i]))
				i++;
			continue;
		}
		curr[word_pos++] = input[i++];
	}
	if (curr)
	{
		add_node(curr, head);
		ft_bzero(curr, word_pos + 1); //probably make a wrapper that bzero's to begin with
		word_pos = 0;
	}
	return (head); // Idk if I need to return head or 1 or smth
}

int	single_qoute_lex(char *input, t_token_list **head, char *curr)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while(input[i] != '\0')
	{
		while(input[i] != '\'')
		{
			curr[j] = input[i]; //see if curr actually gets changed, you might just be sending temporal reference over.
			i++;
			j++;
		}
	}
	if (input[i] == '\0')
	{
		ft_printf("Error: no closing '\n");
		node_free(head); //freeing function for token list that bzeros head/removes all nodes for fresh start next iteration.
		return (-1); //make enum for error handling of incorrect usage
	}
}

int	double_qoute_lex(char *input, t_token_list **head, char *curr)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while(input[i] != '\0')
	{
		while(input[i] != '"')
		{
			curr[j] = input[i]; //see if curr actually gets changed, you might just be sending temporal reference over.
			j++;
			i++;
		}
	}
	if (input[i] == '\0')
	{
		ft_printf("Error: no closing \"\n");
		node_free(head); //freeing function for token list that bzeros head/removes all nodes for fresh start next iteration.
		return (-1); //make enum for error handling of incorrect usage
	}
}



int	neo_parser_processor(char *input, t_token_list **head)
{
	char	*curr;
	size_t	word_pos;
	size_t	i;
	int		escaping;
	int		in_double_quote; //so revamp these bools into one single int using the enums for clarity.
	int		in_single_quote; //then, turn this into a state machine, if in state of quotes, head into a seperate whileloop, return the count we've incremented and go back to the initial whileloop
	int		quote_status;// revamped "bool" here

	quote_status = NOT_IN_QUOTES; // see enums in header
	in_single_quote = 0;
	in_double_quote = 0;
	escaping = 0;
	i = 0;
	word_pos = 0;
	while(input[i] != '\0')
	{
		if (escaping)
		{
			curr[word_pos++] = input[i++];
			escaping = 0;
			continue;
		}
		else if (input[i] == "\\")
		{
			escaping = 1;
			i++;
			continue;
		}
		else if (input[i] == "'" && !in_double_quote)
		{
			in_single_quote = 1;
			i++;
			continue;
		}
		else if (input[i] == '"' && !in_single_quote)
		{
			in_double_quote = 1;
			i++;
			continue;
		}
		else if (input[i] == '|' && !in_double_quote && !in_single_quote)
		{
			add_node(curr, head);
			ft_bzero(curr, word_pos + 1);
			word_pos = 0;
			add_node("|", head);
			while (isspace(input[i]))
				i++;
			continue;
		}
		if (isspace(input[i]) && !in_single_quote && !in_double_quote) //replace with ismetachar, define whether it's whitespace or metachar that needs to be tokenized, generate node accordingly
		{
			if (curr)
			{
				add_node(curr, head);
				ft_bzero(curr, word_pos + 1);
				word_pos = 0;
			}
			while (isspace(input[i]))
				i++;
			continue;
		}
		curr[word_pos++] = input[i++];
	}
	if (curr)
	{
		add_node(curr, head);
		ft_bzero(curr, word_pos + 1);
		word_pos = 0;
	}
	return (head); // Idk if I need to return head or 1 or smth
}

int	ft_ismetachar(char c)
{
	if (isspace(c))
		return (1);
	else if (c == '|')
		return (MS_PIPE);
	else if (c == '&')
		return ()
	// other enums but you get the gist
	
}

int	token_assign(t_token_list **head)
{
	t_token_list	*curr;
	
	curr = *head;
	while(curr != NULL)
	{
		
	}
}

size_t	whitespace_len(char *string)
{
	size_t	i;

	i = 0;
	while (string[i] != '\0' && ft_isspace(string[i]))
		i++;
	return (i);
}

int	parser_processor(char *input, t_token_list *head)
{
	int	i;
	int	j;
	size_t	len;
	char *new_input; // replace it with struct later, initialize struct in main

	j = 0;
	i = 0;
	while(input[i])
	{
		while(isspace(input[i])) // Replace with ft_isspace
		i++;
		if (input[i] == '"')
		{
			len = whitespace_len(input[++i]);
			new_input = (char *)malloc(len);
			while(input[i] != '"')
			{
				new_input[j] = input[i];
			}
			new_input = '\0';
		}
		else if (input[i] > 33 && input[i] < 127)
		{
			ft_strlcpy(new_input, input, whitespace_len(input));
		}
		add_node(new_input, head);
		i += whitespace_len(input[i]);
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

void	add_node(t_parsing_context *par_con,char *string, t_token_list *head)
{
	if (!head)
		head = mk_node(string, head);
	while (head->next != NULL)
		head = head->next; //make sure this doesn't iterate the actual head in your struct, just the pointer to head handed over for this func.
	head->next = mk_node(string, head);
	par_con->tail = head->next;
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
	//returns enum of type for linked list
	//make an array of strings containing the tokens, and then while array
	//strcmp until you find the token, then assign the right token to it and put it in the tree struct
	//in the right order


	if (ft_strlen(string) < 3)
	{
		
	}
}

char	**init_array(int identifier)
{
	// depending on the enum identifier create an array of
	// strings, and stringcompare through the string then give it an enum if it's found
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