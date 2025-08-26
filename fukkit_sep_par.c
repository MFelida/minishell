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
#include <string.h>
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
			par_con->quote = NOT_IN_QUOTES;
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
	par_con->quote = NOT_IN_QUOTES;
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
			something_for_no_error();
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
	par_con->quote = NOT_IN_QUOTES;
	return (0);
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
		while (ft_isalnum(par_con->arg[start + len]) || par_con->arg[start + len] == '_')
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
	VV YOU DO INFACT WANNA DO THIS, append to curr first, then make sure to split it after and only then add it to nodes.
	{
		i = 0;
		while (value[i])
			par_con->curr[par_con->curr_pos++] = value[i++]; 
		free(value);
	}
	{
		if (par_con->quote == NOT_IN_QUOTES)
			var_re_parse(par_con, value); //expand and fully re-lex
		else if (par_con->quote == IN_DOUBLE_QUOTE)
		{
			strjoin(&par_con->curr[par_con->curr_pos], value);
			par_con->curr_pos += len;
		}
		// else if (par_con->quote == IN_SINGLE_QUOTE) // not necessary since we take $ literal in single quotes
		// {
		// 	strjoin(&par_con->curr[par_con->curr_pos], value);
		// 	par_con->curr_pos += len;
		// }
	}
	return (0);
}

int	var_re_parse(t_parsing_context *par_con, char *var)
{
	int i;

	i = 0;
	while(var[i] != '\0')
	{
		// 
	}
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
