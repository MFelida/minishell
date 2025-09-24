/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_and_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:27:17 by ama               #+#    #+#             */
/*   Updated: 2025/09/24 08:22:25 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"
#include <string.h> //only for strcat, remove when strcat is ft_strcat

void	token_assignation(t_token_list *node)
{
	node->type = ft_calloc(sizeof(t_ms_token), 1);
	if (node->type == NULL)
		return ;// exit_func(par_con); or something along those lines.
	if (node_quote_check(node->string))
		return (node_string_assign(node));
	else if (full_meta_check(node->string))
		return (node_meta_assign(node));
	else if (is_cmd(node->string))
		return (node_cmd_assign(node));
	else
		return (node_id_assign(node));
}

void	node_string_assign(t_token_list *node)
{
	node->type->id.type = MS_TOK_IDENTIFIER;
	node->type->id.value = ft_variable_exp_strdup(node->string);
}

void	node_meta_assign(t_token_list *node)
{
	node->type->op.type = MS_TOK_OPERATOR;
	node->type->op.op = op_finder(node->string); //handle heredoc by if node->type->op.op == HEREDOC_TOKEN, suck up next node as eof
}

void	node_cmd_assign(t_token_list *node)
{
	node->type->cmd.type = MS_TOK_COMMAND;
	node->type->cmd.cmd = is_cmd(node->string);
}

void	node_id_assign(t_token_list *node)
{
	node->type->id.type = MS_TOK_IDENTIFIER;
	node->type->id.value = ft_strdup(node->string);
}

char *ft_variable_exp_strdup(char *string)
{
	char	*buff;
	char	*var_name;
	char	*var_val;
	size_t	i;
	size_t	start;

	i = 0;
	buff = (char *)ft_calloc(sizeof(char), 1);
	if (!buff)
		return (NULL); //exit_func(par_con);
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

t_ms_token	return_cmd_token(t_token_list *node)
{
	t_ms_token	token;
	
	token.type = MS_TOK_COMMAND;
	if (node->type->cmd.cmd < MS_CMD_BIN)
		token.cmd.cmd = node->type->cmd.cmd;
	else
		token.cmd.cmd = MS_CMD_ERROR;
	return (token);
}

t_ms_token return_id_token(const char *string)
{
	t_ms_token token;

	if (!string)
		return ((t_ms_token){.type = MS_TOK_ERROR});
	token.type = MS_TOK_IDENTIFIER;
	token.id.value = ft_strdup(string);
	return (token);
}

t_ms_token	return_op_token(int	type)
{
	t_ms_token token;
	
	token.type = MS_TOK_OPERATOR;
	token.op.op = type;
	return (token);
}
