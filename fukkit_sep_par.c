/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fukkit_sep_par.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/08/20 14:26:19 by amel-fou         ###   ########.fr       */
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
		exit (1) //make error handling;

}

int	neo_parser_processor_v3(char *input, t_parsing_context *par_con)
{
	par_con->arg = input; //make sure this equals it to the start of the string/address

	while(par_con->arg[par_con->pos] != '\0')
	{
		while (ft_isspace(par_con->arg[par_con->pos]))
			par_con->arg[par_con->pos];
		if (par_con->arg[par_con->pos] == '"')
		{
			par_con->pos++;
			par_con->quote = IN_DOUBLE_QUOTE;
			neo_double_quote_state(par_con);
			continue ;
		}
		else if (par_con->arg[par_con->pos] == '\'')
		{
			par_con->pos++;
			par_con->quote = IN_SINGLE_QUOTE;
			single_quote_state(par_con);
			continue ;
		}
		else
		{
			argument_state(par_con);
			continue ;
		}
	}

	free(par_con->arg);
	second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

int	neo_parser_processor_v2(char *input, t_token_list **head)
{
	char	*curr;
	size_t	word_pos;
	size_t	i;
	int		escaping;
	int		in_double_quote; //so revamp these bools into one single int using the enums for clarity.
	int		in_single_quote; //then, turn this into a state machine, if in state of quotes, head into a seperate whileloop, return the count we've incremented and go back to the initial whileloop
	int		quote_status;// revamped "bool" here

	t_parsing_context *par_con;

	par_con = (t_parsing_context *)malloc(1 * sizeof(t_parsing_context));
	if (!par_con)
		// free whatever and exit(1);
	par_con->arg = input;
	par_con->head = head; // probably define everything except input and head beforehand, not here.
	par_con->pos = 0;
	par_con->quote = NOT_IN_QUOTES;
	par_con->escaping = NOT_ESCAPING;
	par_con->error = 0;

	quote_status = NOT_IN_QUOTES;
	in_single_quote = 0;
	in_double_quote = 0;
	escaping = 0;
	i = 0;
	word_pos = 0;
	while (par_con->arg[par_con->pos] != '\0')
	{
		while (ft_isspace(par_con->arg[par_con->pos])) //make sure I gave up on metachar idea bc multiple passes
			par_con->arg[par_con->pos++];
		if (par_con->arg[par_con->pos] == '"')
		{
			par_con->pos++;
			par_con->quote = IN_DOUBLE_QUOTE;
			neo_double_quote_state(par_con);
			continue ;
		}
		else if (par_con->arg[par_con->pos] == '\'')
		{
			par_con->pos++;
			par_con->quote = IN_SINGLE_QUOTE;
			single_quote_state(par_con);
			continue ;
		}
		else
		{
			argument_state(par_con);
			continue ;
		}
		//consider checking for an infinite loop
	}
	if (no errors so far)
		second_pass(par_con);
}

int	neo_double_quote_state(t_parsing_context *par_con)
{
	while (par_con->arg[++par_con->pos] != '\0')
	{
		if (par_con->arg[par_con->pos] == '"' && par_con->arg[par_con->pos - 1] != '\\')
		{
			if (par_con->curr)
			{
				par_con->curr[par_con->curr_pos++] = '\0';
				add_node(par_con, par_con->curr, par_con->head);
				par_con->tail->type = MS_TOK_IDENTIFIER;
				// instantly assign identifier bc it is always a quote, when there's quotation, NOT DONE YET!!!!!!!!
			}
			ft_bzero(par_con->curr, ft_strlen(par_con->curr));
			par_con->curr_pos = 0;
			return (0);// return error values
		}
		else if (par_con->arg[par_con->pos] == '$')
		{
			double_quote_var(par_con);
		}
		else
		{
			par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++];
			continue ;
		}
	}
	if (par_con->curr)
	{
		add_node(par_con, par_con->curr, par_con->head);
		// instantly assign identifier bc it is always a quote, when there's quotation, ~NOT DONE YET!!!!!!!!~ DONE NOW >:D 8/8/25
		par_con->tail->type = MS_TOK_IDENTIFIER;
		ft_bzero(par_con->curr, ft_strlen(par_con->curr));
		par_con->curr_pos = 0;
		//ALSO MAYBE ACCOUNT FOR EMPTY CURR, AND HANDLE ERROR NOTATION FOR IT
	}
	return (0); // return error values
}

int	second_pass(t_parsing_context *par_con)
{
	//check for appended things (think: cat|c|at, echo&)
	//perhaps expand variables
	//then second or third pass assign tokens
	//third or fourth pass, make syntax tree struct.
	t_token_list *node;

	node = par_con->head;
	while (node != NULL)
	{
		node_trim_splitter(par_con, node);
	}
}

int	node_trim_splitter(t_parsing_context *par_con, t_token_list *node)
{
	while(node != NULL)
	{
		if (node->type)
		{
			node = node->next;
			continue ;
		}
		meta_splitter(par_con, node);
		node = node->next;
	}
}

void	meta_splitter(t_parsing_context *par_con, t_token_list *node)
{
	while(node->string != '\0')
	{ //START FROM HERE, ADD SUPPORT IN ISMETACHAR FOR RETURNING EVERY TYPE OF ENUM
		if (ismetachar)
	}
}

int	argument_state(t_parsing_context *par_con)
{
	while(par_con->arg[par_con->pos] != '\0')
	{
		if (par_con->arg[par_con->pos] == '"' || par_con->arg[par_con->pos] == '\'' \
			&& ( par_con->pos == 0 || par_con->arg[par_con->pos - 1] == '\\'))
		{
			if (par_con->curr)
			{
				par_con->curr[par_con->curr_pos++] = '\0';
				add_node(par_con, par_con->curr, par_con->head);
			} // instantly assign identifier bc it is always a quote, when there's quotation, NOT DONE YET!!!!!!!! 
			ft_bzero(par_con->curr, ft_strlen(par_con->curr));
			par_con->curr_pos = 0;
			return (0);// return error values
		}
		else if (ft_isspace(par_con->arg[par_con->pos]))
		{
			if (par_con->curr)
			{
				par_con->curr[par_con->curr_pos++] = '\0';
				add_node(par_con, par_con->curr, par_con->head);
			}// instantly assign identifier bc it is always a quote, when there's quotation, NOT DONE YET!!!!!!!!
			ft_bzero(par_con->curr, ft_strlen(par_con->curr));
			par_con->curr_pos = 0;
			return (0);// return error values
		}
	}
}

int	single_quote_state(t_parsing_context *par_con)
{
	while(par_con->arg[par_con->pos] != '\'')
	{
		par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++];
	}
	add_node(par_con, par_con->curr, par_con->head);
	// instantly assign identifier bc it is always a quote, when there's quotation, NOT DONE YET!!!!!!!!
	par_con->tail->type = MS_TOK_IDENTIFIER;
	//ALSO MAYBE ACCOUNT FOR EMPTY CURR, AND HANDLE ERROR NOTATION FOR IT
	ft_bzero(par_con->curr, ft_strlen(par_con->curr));
	par_con->curr_pos = 0;
}

int	var_expansion(t_parsing_context *par_con)
{
	char *varname;
	char *value;
	int	start;
	int	len;
	int	i;

	len = 0;
	i = 0;
	if (par_con->arg[par_con->pos + 1] == '?')
	{
		value = mike_exit_state_itoa(g_exit_state); //examle
		par_con->pos += 2;
	}
	else
	{
		start = par_con->pos + 1;
		while (ft_isalnum(par_con->arg[start + len] || par_con->arg[start + len] == '_'))
			len++;
		if (len == 0)
		{
			par_con->curr[par_con->curr_pos++] = '$';
			par_con->pos++;
			return (0);
		}
		varname = ft_substr(par_con->arg, start, len);
		value = mike_var_find_func(varname);
		free(varname);
		par_con->pos = start + len;
	}
	if (value)
	{
		i = 0;
		while (value[i])
			par_con->curr[par_con->curr_pos++] = value[i++];
		free(value);
	}
	var_re_parse(par_con);
	return (0);
}

int var_re_parse(t_parsing_context *par_con)
{
	//make re-parsing func in case there's several tokens.
	int	i;

	i = 0;
	while(par_con->tail->string[i] != '\0')
	{
		if (par_con->tail->string[i]) == ' ' || ismetachar(par_con->tail->string[i])
			split the input, idk how you should split it yet
	}
}

int	var_node_splitter(t_parsing_context *par_con)
{
	
}

int	double_quote_var(t_parsing_context *par_con)
{
	char	*varname;
	int		i;
	int		temp_pos;
	const char *ret;

	temp_pos = (par_con->pos + 1);
	i = 0;
	while(!isspace(par_con->arg[par_con->pos]))
	{
		par_con->arg[par_con->pos++];
		i++;
	}
	varname = ft_substr(&par_con->arg[par_con->pos], 0, i);
	ret = ms_get_env(varname); //replace with mike's env finder later
	i = 0;
	while (ret[i] != '\0')
	{
		par_con->arg[par_con->pos] = ret[i];
		par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++];
		i++;
	}
	return (ret);
}

int	double_quote_state(t_parsing_context *par_con)
{
	while (par_con->arg[++par_con->pos] != '\0')
	{
		if (par_con->arg[par_con->pos] == '"' && par_con->arg[par_con->pos - 1] != '\\')
		{
			if (par_con->curr)
			{
				par_con->curr[par_con->curr_pos++] = '\0';
				add_node(par_con, par_con->curr, par_con->head);
				par_con->tail->type = MS_TOK_IDENTIFIER;
				// instantly assign identifier bc it is always a quote, when there's quotation, NOT DONE YET!!!!!!!!
			}
			ft_bzero(par_con->curr, ft_strlen(par_con->curr));
			par_con->curr_pos = 0;
			return (0);// return error values
		}
		if (par_con->arg[par_con->pos] == '\\')
		{
			par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++]; 
			par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++];
			continue ;
		}
		else
		{
			par_con->curr[par_con->curr_pos++] = par_con->arg[par_con->pos++];
			continue ;
		}
	}
	add_node(par_con, par_con->curr, par_con->head);
	// instantly assign identifier bc it is always a quote, when there's quotation, ~NOT DONE YET!!!!!!!!~ DONE NOW >:D 8/8/25
	par_con->tail->type = MS_TOK_IDENTIFIER;
	ft_bzero(par_con->curr, ft_strlen(par_con->curr));
	par_con->curr_pos = 0;
	//ALSO MAYBE ACCOUNT FOR EMPTY CURR, AND HANDLE ERROR NOTATION FOR IT

	return (0); // return error values
}


//THIS IS NOT NEEDED ANYMORE AS I MADE MAKE TAIL BE THE MOST RECENT NODE EVERY TIME SO THAT I CAN INSTANTLY CHANGE ITS TYPE EVERY TIME >:D

// void	quote_assign(t_token_list **head) //Idk if double deref is correct here when just passing par_con->head
// {
// 	t_token_list *node;

// 	node = *head;
// 	while(node->next != NULL)
// 		node = node->next;
// 	node->type = t_ms_id_token; //change type to union, not int? and then assign id token?? with information?? 
// 	//for example here, list the quote status that is included in the token enum/struct
// }

int	neo_parser_processor(char *input, t_token_list **head)
{
	char	*curr;
	size_t	word_pos;
	size_t	i;
	int		escaping;
	int		in_double_quote; //so revamp these bools into one single int using the enums for clarity.
	int		in_single_quote; //then, turn this into a state machine, if in state of quotes, head into a seperate whileloop, return the count we've incremented and go back to the initial whileloop
	int		quote_status;// revamped "bool" here

	t_parsing_context *parsing_context;

	parsing_context = (t_parsing_context *)malloc(1 * sizeof(t_parsing_context));
	if (!parsing_context)
		// free whatever and exit(1);
	parsing_context->argument = input;
	parsing_context->head = head; // probably define everything except input and head beforehand, not here.
	parsing_context->pos = 0;
	parsing_context->quote_status = NOT_IN_QUOTES;
	parsing_context->escaping = NOT_ESCAPING;

	quote_status = NOT_IN_QUOTES; // see enums in header
	in_single_quote = 0;
	in_double_quote = 0;
	escaping = 0;
	i = 0;
	word_pos = 0;
	while(parsing_context->argument[parsing_context->pos] != '\0')
	{
		if (parsing_context->escaping = ESCAPING) //escaping, double escaping and single escaping is all 1 so it's gucci
		{
			curr[word_pos++] = input[i++];
			parsing_context->escaping = 0;
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
		else if (input[i] == '|' && !in_double_quote && !in_single_quote) //consider splitting appended tokens like |cat in a next stage after lexing.\
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

int	ismetachar(char c)
{
	int	i;

	while(METACHARS[i])
	{
		if (METACHARS[i] == c)
			return (1);
		i++;
	}
	return (0);
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

parse_meta_char(char *input, t_token_list **head)
{
	int	i;
	int	j;
	char *curr;

	j = 0;
	i = 0;
	while(input[i] != '\0' && (ismetachar(input[i]) || isspace(input[i])))
	{
		while(isspace(input[i]))
		{
			i++;
		}
		if (ismetachar(input[i]))
		{
			curr[j++] = input[i];
			if (ismetachar(input[i + 1]))
			{
				curr[j++] = input[i + 1];
				add_node(curr, head); //ok this won't work bc it needs a string, make a curr.
				// also, make it assign it a metachar flag as well
				i++;
			}
			i++; //iteration is gonna be weird here, fix it
		}
	}
	return (i);
}

int	JUST_PARSE_IT_BRO(char *input, t_token_list **head)
{
	int	i;
	char *curr;
	int	temp;

	i = 0;
	while(input[i] != '\0')
	{
		if(ismetacharacter(input[i]))
		{
			temp += parse_meta_char(input[i], head);
			if (temp < 0) //invalid character spotted
			{
				free_head(head);
				exit (1); // for instance
			}
		}
	}
}

