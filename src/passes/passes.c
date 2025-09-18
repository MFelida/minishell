/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_pass.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:17:45 by ama               #+#    #+#             */
/*   Updated: 2025/09/18 12:41:02 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

#include <stdio.h>

int	first_pass(char *input, t_parsing_context *par_con) //probably change name to "first pass" =w=
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
	//test_first_pass(par_con);
	second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

int	second_pass(t_parsing_context *par_con)
{
	t_token_list *node;

	node = par_con->head;
	trim_space_nodes(par_con);
	while(node != NULL)
	{
		token_assignation(node);
		node = node->next;
	}
	//check for errors maybe idk
	//test_second_pass(par_con);
	third_pass(par_con);
	return (0); //or exit state
}

void	third_pass(t_parsing_context *par_con)
{
	t_token_list	*node;
	t_token_list	*starting_node;
	t_parse_tree	*command_node;
	t_parse_tree	*operator_node;
	size_t			node_count;

	node_count = 0;
	node = par_con->head;
	starting_node = node;
	while(node != NULL)
	{
		if (node->type->type == MS_TOK_OPERATOR)
		{
			command_node = assemble_command_node(starting_node, node_count);
			operator_node = new_tree((t_ms_token){.op = node->type->op}); //check if this is norm ok
			new_child(operator_node, command_node);
			attach_to_tree(par_con, operator_node);
			starting_node = node->next;
			node_count = 0;
		}
		else
			node_count++;
		node = node->next;
	}
	if (node_count > 0)
	{
		command_node = assemble_command_node(starting_node, node_count);
		attach_to_tree(par_con, command_node);
	}
	//lead into mike's part
}

