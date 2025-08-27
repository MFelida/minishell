/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thelongway.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/08/27 15:42:10 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_header.h"

void	init_parcon(t_parsing_context *par_con, t_token_list **head)
{
	par_con->head = head; // probably define everything except input and head beforehand, not here.
	par_con->pos = 0;
	par_con->quote = NOT_IN_QUOTES;
	par_con->escaping = NOT_ESCAPING;
	par_con->error = 0;
	par_con->tail = NULL;
	par_con->curr = (char *)malloc(sizeof(char) * 3e6);
	if (!par_con->curr)
		exit (1); //make error handling;
    par_con->start = par_con->arg[par_con->pos];
}

int	neo_parser_processor_v3(char *input, t_parsing_context *par_con)
{
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
	}

	free(par_con->arg);
	second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

int metastate(t_parsing_context *par_con)
{
    ft_substr(par_con->arg, par_con->start, (par_con->start - par_con->pos)); //maybe make a wrapper that sets the start to pos.
	par_con->start = par_con->pos; //like this
	if ((ismetachar(par_con->arg[par_con->pos + 1])) \
	&& !ft_isspace(par_con->arg[par_con->pos]) \
	&& !ft_isspace(par_con->arg[par_con->pos + 1])) 
	{
		//check if it's << etc, if it's only whitespace until on more metacharacters, do while ft_isspace like below.
		par_con->pos += 2;
	}
	while (ft_isspace(par_con->arg[par_con->pos]))
		par_con->pos++;
}


int	ismetachar(char c)
{
	int i;

	i = 0;
	while (i < 15)
	{
		if (c = METACHARS[i])
			return (1);
		i++;
	}
	return (0);
}
