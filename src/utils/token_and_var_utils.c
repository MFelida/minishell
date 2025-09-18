/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_and_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:27:17 by ama               #+#    #+#             */
/*   Updated: 2025/09/18 12:43:31 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"
#include <string.h> //only for strcat, remove when strcat is ft_strcat

void	token_assignation(t_token_list *node)
{
	int fd_heredoc;
						//STILL NEED TO HANDLE HEREDOC EOF
	fd_heredoc = -1; //for now, talk to mike abt what to do with it
	node->type = malloc(sizeof(t_ms_token));
	if (!node->type)
		// exit_func(par_con); or something along those lines.
	ft_bzero(node->type, sizeof(t_ms_token));
	if (node_quote_check(node->string))
	{
		node->type->id.type = MS_TOK_IDENTIFIER;
		node->type->id.value = ft_variable_exp_strdup(node->string);
		//node->string = example_trim(node->string);//trim quotes, probably account for "$foo"bar type nodes in third pass on unquoted identifiers.
		return ;
	}
	else if (full_meta_check(node->string))
	{
		node->type->op.type = MS_TOK_OPERATOR;
		node->type->op.op = op_finder(node->string);
		if (node->type->op.op == MS_HEREDOC)
		{
			//identify EOF (which will be node->next's string)
			//take into account if it's qouted or not bc weird behaviour and appending
			// go back to readline until EOF
			// send all the taken in lines into file
			node->type->op.value = fd_heredoc;
		}
	}
	else if (is_cmd(node->string)) //func thta checks for each builtin/cmd whatever
	{
		node->type->cmd.type = MS_TOK_COMMAND;
		node->type->cmd.cmd = is_cmd(node->string);
	}
	else
	{
		//its an identifier without quotes.
		node->type->id.type = MS_TOK_IDENTIFIER; //maybe rpelace with "word" enum for clarity in parsing between uknown, and known identifiers.
		node->type->id.value = ft_strdup(node->string);
	}
}

char *ft_variable_exp_strdup(char *string)
{
	char	*buff;
	char	*var_name;
	char	*var_val;
	size_t	i;
	size_t	start;

	i = 0;
	buff = (char *)malloc(sizeof(char) * 1);
	if (!buff)
		return (NULL); //exit_func(par_con);
	buff[0] = '\0';
	while (string[i])
	{
		if (string[i] == '$')
		{
			i++;
			start = i;
			while (string[i] && !ismetachar(string[i]) && string[i] != '"' && string[i] != '\'')
				i++;
			var_name = ft_substr(string, start, i - start);
			var_val = getenv(var_name);
			free(var_name);
			if (var_val)
				strcat(buff, var_val); //replace with ft_strcat
		}
		else
		{
			start = i;
			while (string[i] && string[i] != '$')
				i++;
			var_name = ft_substr(string, start, i - start);
			strcat(buff, var_name); //replace with ft_strcat
			free(var_name);
		}
	}
	return (buff);
}
