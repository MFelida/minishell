/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thelongway.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/01 16:15:33 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/parsing_header.h"
#include <unistd.h>

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t')
		return (1);
	return (0);
}

void	init_parcon(t_parsing_context *par_con)
{
	par_con->head = NULL; // probably define everything except input and head beforehand, not here.
	par_con->pos = 0;
	par_con->quote = NOT_IN_QUOTES;
	par_con->escaping = NOT_ESCAPING;
	par_con->error = 0;
	par_con->tail = NULL;
	par_con->curr = (char *)malloc(sizeof(char) * 3e6);
    par_con->start = par_con->pos;
	if (!par_con->curr)
		exit (1); //make error handling;
}

int	neo_parser_processor_v3(char *input, t_parsing_context *par_con)
{
	char *test;
	par_con->arg = input; //make sure this equals it to the start of the string/address

	while(par_con->arg[par_con->pos] != '\0')
	{
		if (ismetachar(par_con->arg[par_con->pos]))
		{
			metastate(par_con);
			//we're gonna split on metacharacters, also split whitespace into its own nodes, this is specifically for
			// when we expand a variable, if it's foo$bar it needs to append to foo. outside of double quote it also needs to append rightward.
			//we will check if the previous node is whitespace or not and act accordingly on varriable expansion, maybe merg to prev if foo$bar
			//metacharacters will be SUBSTRINGed into their own nodes. Then tokenization will be way easier.
			//need to check pos + 1 when substringing in case it's << for example. good luck ama.
		}
		else
			par_con->pos++;
	}
	if (par_con->pos > par_con->start) //check if there is input after metacharacters that still needs to be malloced
	{
		test = ft_substr_wrapper(par_con);
		if (!test)
			// exit_func(par_con);
		add_node(par_con, test, par_con->head);
		free(test);
	}
	//free(par_con->arg);
	//second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

int metastate(t_parsing_context *par_con)
{
    //ft_substr(par_con->arg, par_con->start, (par_con->start - par_con->pos)); //maybe make a wrapper that sets the start to pos.
	char	*test;

	test = ft_substr_wrapper(par_con);
	if (!test)
		// exit_func(par_con); //maybe go back to readline, maybe just exit out since a literal bitesize malloc failed.
	add_node(par_con, test, par_con->head); //like this
	free(test);
	if (par_con->arg[par_con->pos] != '\0' &&
		par_con->arg[par_con->pos + 1] != '\0' &&
		ismetachar(par_con->arg[par_con->pos + 1]) &&
		!ft_isspace(par_con->arg[par_con->pos]) &&
		!ft_isspace(par_con->arg[par_con->pos + 1]))
	{
		write(1, "test\n", 5);
		//check if it's << etc, if it's only whitespace until on more metacharacters, do while ft_isspace like below.
		par_con->pos += 2;
		test = ft_substr_wrapper(par_con);
		if (!test)
			// exit_func(par_con);
		add_node(par_con, test, par_con->head);
		free(test);
	}
	while (ft_isspace(par_con->arg[par_con->pos]) && par_con->arg[par_con->pos] != '\0')
	{
		par_con->pos++;
	}
	if (ft_isspace(par_con->arg[par_con->pos - 1]))
	{
		test = ft_substr_wrapper(par_con);
		add_node(par_con, test, par_con->head);
		free(test);
	}
	return (0);
}

int	ismetachar(char c)
{
	int i;

	i = 0;
	while (i < 16)
	{
		if (c == METACHARS[i])
			return (1);
		i++;
	}
	return (0);
}


char	*ft_substr_wrapper(t_parsing_context *par_con)
{
	char *ret;

	if (par_con->pos > par_con->start)
		ret = ft_substr(par_con->arg, par_con->start, par_con->pos - par_con->start);
	else
		ret = NULL;

	par_con->start = par_con->pos;
	return (ret);
}

///////////////////////////////// TESTING SECTION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

#include <stdio.h>

void	add_node(t_parsing_context *par_con,char *string, t_token_list *head)
{
	if (!head)
		par_con->head = mk_node(string);
	while (head->next != NULL)
		head = head->next; //make sure this doesn't iterate the actual head in your struct, just the pointer to head handed over for this func.
	head->next = mk_node(string);
	par_con->tail = head->next;
}

t_token_list	*mk_node(char *string)
{
	t_token_list *node;

	node = (t_token_list *)malloc(sizeof(t_token_list));
	node->string = string;
	//node->type = typefinder(string);
	node->type = NULL;
	node->next = NULL;
	return (node);
}

void print_tokens(t_parsing_context *par_con) 
{
	t_token_list *cur = par_con->head;
	while (cur)
	{
		printf("[%s]\n", cur->string);
		cur = cur->next;
	}
}

int	main(void)
{
	t_parsing_context *par_con;
	char *string;

	par_con = (t_parsing_context *)malloc(1 * sizeof(t_parsing_context));
	string = "test ing|test|ing";

	init_parcon(par_con);
	neo_parser_processor_v3(string, par_con);
	print_tokens(par_con);

}
//work to make it testable when u next work, other than that, w progress