/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:17:45 by ama               #+#    #+#             */
/*   Updated: 2025/09/24 13:37:02 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

int	first_pass(t_parsing_context *par_con)
{
	char	*test;

	while (par_con->arg[par_con->pos])
	{
		if (par_con->arg[par_con->pos] == '\''
			|| par_con->arg[par_con->pos] == '"')
			quote_state_switch(par_con);
		if (ismetachar(par_con->arg[par_con->pos])
			&& par_con->quote == NOT_IN_QUOTES)
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
	//test_first_pass(par_con); //TESTER
	second_pass(par_con);
	return (0); //or exit state, figure this out later along the line
}

int	second_pass(t_parsing_context *par_con)
{
	t_token_list	*node;

	trim_space_nodes(par_con);
	if (par_con->head == NULL)
	{
		return (1);
		// free_token_list(par_con->head); //this might've broken stuff
	}
	node = par_con->head;
	while (node != NULL)
	{
		token_assignation(node);
		node = node->next;
	}
	//check for errors maybe idk
	if (par_con->head != NULL)
	{
		//test_second_pass(par_con);
		third_pass(par_con);
	}
	return (0); //or exit state
}

void	third_pass(t_parsing_context *par_con)
{
	t_token_list	*node;
	t_token_list	*starting_node;
	size_t			node_count;

	node_count = 0;
	node = par_con->head;
	starting_node = node;
	while (node != NULL)
	{
		if (node->type->type == MS_TOK_OPERATOR)
		{
			operator_state(par_con, node, starting_node, node_count);
			starting_node = node->next;
			node_count = 0;
		}
		else
			node_count++;
		node = node->next;
	}
	if (node_count > 0)
		extra_node_tree(par_con, starting_node, node_count);
	// test_third_pass(par_con);
	//lead into mike's part
	//free_parse_tree(par_con->root);
}

