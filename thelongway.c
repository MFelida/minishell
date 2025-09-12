/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thelongway.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/08 15:13:43 by amel-fou         ###   ########.fr       */
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
	par_con->head = NULL;
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
		if (par_con->arg[par_con->pos] == '\'' || par_con->arg[par_con->pos] == '"')
			quote_state_switch(par_con);
		if (ismetachar(par_con->arg[par_con->pos]) && par_con->quote == NOT_IN_QUOTES)
			metastate(par_con);
		else
			par_con->pos++;
	}
	if (par_con->pos >= par_con->start)
	{
		test = ft_substr_wrapper(par_con);
		if (test && *test)
			add_node(par_con, test);
		if (test)
			free(test);
	}
	free(par_con->arg);
	second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

void	quote_state_switch(t_parsing_context *par_con)
{
	if (par_con->arg[par_con->pos] == '\'' && par_con->quote == NOT_IN_QUOTES)
	{
		par_con->quote = IN_SINGLE_QUOTE;
		return ;
	}
	else if (par_con->arg[par_con->pos] == '\'' && par_con->quote == IN_SINGLE_QUOTE)
	{
		par_con->quote = NOT_IN_QUOTES;
		return ;
	}
	else if (par_con->arg[par_con->pos] == '"' && par_con->quote == NOT_IN_QUOTES)
	{
		par_con->quote = IN_DOUBLE_QUOTE;
		return ;
	}
	else if (par_con->arg[par_con->pos] == '"' && par_con->quote == IN_DOUBLE_QUOTE)
	{
		par_con->quote = NOT_IN_QUOTES;
		return ;
	}
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
	// else;
		// exit_func(par_con); //maybe go back to readline, maybe just exit out since a literal bitesize malloc failed.
	if (test)
		free(test);
	if (par_con->arg[par_con->pos + 1] &&
		ismetachar(par_con->arg[par_con->pos + 1]) &&
		!ft_isspace(par_con->arg[par_con->pos]) &&
		!ft_isspace(par_con->arg[par_con->pos + 1]))
			return(double_meta_consumption(par_con, buf));

	meta_consumption(par_con, buf);
	while ( par_con->arg[par_con->pos] && ft_isspace(par_con->arg[par_con->pos]))
		par_con->pos++;
	if (par_con->pos > 0 && ft_isspace(par_con->arg[par_con->pos - 1]))
	{
		test = ft_substr_wrapper(par_con);
		if (test && *test)
			add_node(par_con, test);
		//else;
			//exit_func(par_con);
		if (test)
			free(test);
	}
	//second_pass(par_con);
	return (0);
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

int	second_pass(t_parsing_context *par_con)
{
	t_token_list *node;

	node = par_con->head;
	while(node != NULL)
	{
		token_assignation(node);
		node = node->next;
	}
	//check for errors maybe idk
	third_pass(par_con);
}

void	token_assignation(t_token_list *node) //https://www.ibm.com/docs/en/aix/7.1.0?topic=concepts-shells-terminology
{
	int	i;
	int	fd_heredoc;
						//STILL NEED TO EXPAND LOOSE VARIABLES OUTSIDE OF QUOTES AND HANDLE HEREDOC EOF
	i = 0;
	node->type = malloc(sizeof(t_ms_token));
	if (!node->type)
		// exit_func(par_con); or something along those lines.
	if (node_quote_check(node->string))
	{
		node->type->id.type = MS_TOK_IDENTIFIER;
		node->type->id.value = ft_variable_exp_strdup(node->string);
		node->string = example_trim(node->string);//trim quotes, probably account for "$foo"bar type nodes in third pass on unquoted identifiers.
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
	// else if (is_cmd(node->string)) //func thta checks for each builtin/cmd whatever
	// {
	// 	node->type->cmd.type =
	// }
	else
	{
		//its an identifier without quotes.
		node->type->id.type = MS_TOK_IDENTIFIER; //maybe rpelace with "word" enum for clarity in parsing between uknown, and known identifiers.
		node->type->id.value = ft_strdup(node->string);
	}
}

int	op_finder(char *string)
{
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
}

char	*example_trim(char *string)
{
	char *ret;

	ret = ft_substr(string, 1, ft_strlen(string) - 2);
	return (ret);
}

int	is_cmd(char *string)
{
	// I guess just if/else statements out the wazoo, maybe return correct enum for identifier
	//builtins are commands
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
	//figure out how to do MS_CMD_BIN
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
	if (string[0] == '\'' && string[i] == '\'')
		return (1);
	else if (string[0] == '"' && string[i] == '"')
		return (1);
	else
		return (0);
}

char *ft_variable_exp_strdup(char *string)
{
	int		i;
	int		boo;
	char	*buf;
	char	*var;

	boo = 0;
	i = 0;
	while(string[i])
	{
		if (string[i] == '$')
			boo = 1;
		i++;
	}
	if (boo && string[0] == '"')
	{
		while (string[i + 1] != '$')
			i++;
		var = (&string[i + 1]); //or i+2 idk how mike's fetch works.
		buf = ft_substr(string, 0, i);
		buf = ft_strcat(buf, mike_var_fetch(var)); //make wrapper that allocates right amount?
		i += 2;
		while (!ismetachar(string[i]))
			i++;
		i++;
		buf = ft_strcat(buf, &string[i]); //make wrapper that allocates right amount?
	}
	else
		buf = ft_substr(string, 0, ft_strlen(string));
	//maybe free string here? Idk
	//also don't forget to malloc check, ideally just make a wrapper that malloc checks and exits
	//coherently
	return (buf);
}

void	third_pass(t_parsing_context *par_con)
{
	//WHOOPS IGNORE THE COMMENT BELOW, you still need to contextually assign cmds based on metachars,
	//and figure out what the remaining cases are with unquoted, non cmd strings.
	//for example one of them is "$FOO"bar, where bar needs to append to expanded foo WITHOUT quotes
	//also take into account specific heredoc cases.
	//PROBABLY CHECK FIRST FOR BUILTINS, that's an easy assign.
	t_token_list	*node;
	size_t			node_count;
	size_t			starting_node;

	node_count = 0;
	starting_node = 0;
	node = par_con->head;
	while(node != NULL)
	{
		//idk how to check for foo"$ba"r yet either or what to do with it.
		if (node->type == MS_TOK_OPERATOR)
		{
			arrange_command(par_con, node_count); boekenlegger type beat so I see it next time;
			node_count = 0;
			
		}
		node = node->next;
		node_count++;
	}

	//RETROSPECTIVELY, THIS VVVVV IS PROBABLY FOURTH_PASS
	//tokens have been assigned, now you need to structure a syntax tree, 
	//find out how to order each token correctly, actually building the tree
	//itself shouldn't be too hard. gl.
	
}

t_parse_tree	*new_tree(t_ms_token tok)
{
	t_parse_tree *new;
	new = (t_parse_tree *)malloc(sizeof(*new));
	new->tok = tok;
	new->child_nodes = NULL;
	return (new);
}

void	new_child(t_parse_tree *tree, t_parse_tree *sapling)
{
	size_t count;

	count = 0;
	if (tree->child_nodes)
	{
		while (tree->child_nodes[count])
			count++;
	}
	tree->child_nodes = ft_realloc(tree->child_nodes, sizeof(t_parse_tree *) * (count + 2));
	tree->child_nodes[count] = sapling;
	tree->child_nodes[count + 1] = NULL;
}

void	arrange_command(t_parsing_context *par_con, int	node_count)
{
	while(node != )
}

///////////////////////////////// TESTING SECTION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

#include <stdio.h>

void	add_node(t_parsing_context *par_con, char *string)
{
	t_token_list *new_node;

	if (!string || *string == '\0')
		return ;
	new_node = mk_node((char *)string);
	if (!new_node)
		return ;
		//exit_func(par_con);
	if (!par_con->head)
	{
		par_con->head = new_node;
		par_con->tail = new_node;
	}
	else
	{
		par_con->tail->next = new_node;
		par_con->tail = new_node;
	}
}

t_token_list	*mk_node(char *string)
{
	t_token_list *node;

	node = (t_token_list *)malloc(sizeof(* node));
	if (!node)
		return (NULL);
	node->string = ft_strdup(string);
	if(!node->string)
	{
		free(node);
		//exit_func(par_con);	
	}
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
	string = "<< test ing|test|ing $test|VAR \"te|st\" | 'bru|h'";

	init_parcon(par_con);
	neo_parser_processor_v3(string, par_con);
	print_tokens(par_con);

}
