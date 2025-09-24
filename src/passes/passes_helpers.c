/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passes_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:04:13 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/24 10:04:48 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

void	operator_state(t_parsing_context *par_con, t_token_list *node, \
						t_token_list *starting_node, size_t node_count)
{
	t_parse_tree	*command_node;
	t_parse_tree	*operator_node;

	command_node = assemble_command_node(starting_node, node_count);
	operator_node = new_tree(return_op_token(node->type->op.op));
	new_child(operator_node, command_node);
	attach_to_tree(par_con, operator_node);
}

void	extra_node_tree(t_parsing_context *par_con, t_token_list *starting_node, size_t node_count)
{
	t_parse_tree *command_node;

	command_node = assemble_command_node(starting_node, node_count);
	attach_to_tree(par_con, command_node);
}