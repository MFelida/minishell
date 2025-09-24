/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:25:57 by ama               #+#    #+#             */
/*   Updated: 2025/09/24 09:50:34 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"
#include <unistd.h>

int	op_finder(char *string)
{
	if (ft_strncmp(string, "||", 2) == 0)
	//error out or handle bonus
		return (MS_TOK_ERROR);
	else if (ft_strncmp(string, "&&", 2) == 0)
	//error out or handle bonus
		return (MS_TOK_ERROR);
	if (ft_strncmp(string, "<", ft_strlen(string)) == 0)
		return (MS_FILE_INPUT);
	else if (ft_strncmp(string, ">", ft_strlen(string)) == 0)
		return (MS_FILE_OUTPUT);
	else if (ft_strncmp(string, ">>", ft_strlen(string)) == 0)
		return (MS_FILE_APPEND);
	else if (ft_strncmp(string, "<<", ft_strlen(string)) == 0)
		return (MS_HEREDOC);//don't forget to check next node for eof
	else if (ft_strncmp(string, "|", ft_strlen(string)) == 0)
		return (MS_PIPE);
	else if (ft_strncmp(string, "$", ft_strlen(string)) == 0)
		return (MS_ENV_VAR); //don't forget to check next node.
	else
		return (MS_TOK_ERROR);
}

char	*example_trim(char *string)
{
	char *ret;

	ret = ft_substr(string, 1, ft_strlen(string) - 2);
	return (ret);
}

int	is_cmd(char *string)
{
	if (ft_strncmp(string, "echo", ft_strlen(string)) == 0)
		return (MS_CMD_ECHO);
	else if (ft_strncmp(string, "cd", ft_strlen(string)) == 0)
		return (MS_CMD_CD);
	else if (ft_strncmp(string, "pwd", ft_strlen(string)) == 0)
		return (MS_CMD_PWD);
	else if (ft_strncmp(string, "export", ft_strlen(string)) == 0)
		return (MS_CMD_EXPORT);
	else if (ft_strncmp(string, "unset", ft_strlen(string)) == 0)
		return (MS_CMD_UNSET);
	else if (ft_strncmp(string, "env", ft_strlen(string)) == 0)
		return (MS_CMD_ENV);
	else if (ft_strncmp(string, "exit", ft_strlen(string)) == 0)
		return (MS_CMD_EXIT);
	else if (access(string, X_OK) == 0)
		return (MS_CMD_BIN);
	else
		return (MS_CMD_ERROR);
}

int	full_meta_check(char *string)
{
	int	i;

	i = 0;
	while(string[i])
	{
		if (!ismetachar(string[i]))
			return (0);
		i++;
	}
	return (1);
}

int	node_quote_check(char *string)
{
	int	i;

	i = 0;
	while(string[i])
		i++;
	if (string[0] == '\'' && string[i - 1] == '\'')
		return (1);
	else if (string[0] == '"' && string[i - 1] == '"')
		return (1);
	else
		return (0);
}

char	*ft_substr_wrapper(t_parsing_context *par_con)
{
	char *ret;

	if (par_con->pos > par_con->start)
	{
		ret = ft_substr(par_con->arg, par_con->start, par_con->pos - par_con->start);
		if (!ret)
			return NULL; //figure out how to error handle, malloc failed so need to go back to readline
	}
	else
		ret = NULL;
	par_con->start = par_con->pos;
	return (ret);
}

int	double_meta_consumption(t_parsing_context *par_con, char *buf)
{
	buf[0] = par_con->arg[par_con->pos];
	buf[1] = par_con->arg[par_con->pos + 1];
	buf[2] = '\0';
	add_node(par_con, buf);
	par_con->pos += 2;
	par_con->start = par_con->pos;
	free(buf);
	return (0);
}

void	meta_consumption(t_parsing_context *par_con, char *buf)
{
	buf[0] = par_con->arg[par_con->pos];
	buf[1] = '\0';
	add_node(par_con, buf);
	par_con->pos++;
	par_con->start = par_con->pos;
}

void	quote_state_switch(t_parsing_context *par_con)
{
	if (par_con->arg[par_con->pos] == '\'' && par_con->quote == NOT_IN_QUOTES)
		par_con->quote = IN_SINGLE_QUOTE;
	else if (par_con->arg[par_con->pos] == '\'' && par_con->quote == IN_SINGLE_QUOTE)
		par_con->quote = NOT_IN_QUOTES;
	else if (par_con->arg[par_con->pos] == '"' && par_con->quote == NOT_IN_QUOTES)
		par_con->quote = IN_DOUBLE_QUOTE;
	else if (par_con->arg[par_con->pos] == '"' && par_con->quote == IN_DOUBLE_QUOTE)
		par_con->quote = NOT_IN_QUOTES;
	if (par_con->arg[par_con->pos + 1] == '\0' && par_con->quote != NOT_IN_QUOTES)
	{
		//error for unclosed quotes.
		par_con->error = 1;
	}
}

int	is_non_space_double_meta(t_parsing_context *par_con)
{
	if (par_con->arg[par_con->pos + 1] &&
		ismetachar(par_con->arg[par_con->pos + 1]) &&
		!ft_isspace(par_con->arg[par_con->pos]) &&
		!ft_isspace(par_con->arg[par_con->pos + 1]))
			return (1);
	return (0);
}

int metastate(t_parsing_context *par_con)
{
	char	*test;
	char	*buf;

	buf = malloc(3);
	if (!buf)
		return (1);//exit_func(par_con);
	test = ft_substr_wrapper(par_con);
	if (test && *test)
		add_node(par_con, test);
	if (test)
		free(test);
	if (is_non_space_double_meta(par_con))
			return(double_meta_consumption(par_con, buf));
	if (par_con->arg[par_con->pos] == '$')
		return (var_state(par_con, test, buf));
	meta_consumption(par_con, buf);
	while ( par_con->arg[par_con->pos] && ft_isspace(par_con->arg[par_con->pos]))
		par_con->pos++;
	if (par_con->pos > 0 && ft_isspace(par_con->arg[par_con->pos - 1]))
		space_state(par_con, test);
	//second_pass(par_con);
	free(buf);
	return (0);
}

int	space_state(t_parsing_context *par_con, char *test)
{
	test = ft_substr_wrapper(par_con);
	if (test && *test)
		add_node(par_con, test);
	//else;
		//exit_func(par_con);
	if (test)
		free(test);
	return (0);
}

int	var_state(t_parsing_context *par_con, char *test, char *buf)
{
	size_t	start;

	start = par_con->pos;
	par_con->pos++;
	while(par_con->arg[par_con->pos] && (ft_isalnum(par_con->arg[par_con->pos])))
		par_con->pos++;
	test = ft_substr(par_con->arg, start, par_con->pos - start);
	if (!test)
		return (-1); //free par_con and go back to readline after printing error, probably make memory error func
	add_node(par_con, test);
	free(test);
	free(buf);
	return (0);
}

int	ismetachar(char c)
{
	int	i;

	i = 0;
	while(METACHARS[i])
	{
		if (METACHARS[i] == c)
			return (1);
		i++;
	}
	if (ft_isspace(c))
		return (1);	
	return (0);
}
