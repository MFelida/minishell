/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:21:58 by ama               #+#    #+#             */
/*   Updated: 2025/09/24 08:28:17 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

t_parse_tree	*assemble_command_node(t_token_list *start, size_t count)
{
	t_parse_tree	*operator_node;
	t_parse_tree	*child_node;
	t_token_list	*curr;
	size_t			i;

	i = 0;
	curr = start;
	if (curr->type->type == MS_TOK_COMMAND)
	{
		operator_node = new_tree(return_cmd_token(curr));
		curr = curr->next;
		count--;
	}
	else
		operator_node = new_tree(return_op_token(curr->type->op.op));
	while (i < count || (count == 0 && i == 0))
	{
		child_node = new_tree(return_id_token(curr->type->id.value));
		new_child(operator_node, child_node);
		curr = curr->next;
		i++;
	}
	return (operator_node);
}

void	attach_to_tree(t_parsing_context *par_con, t_parse_tree *node)
{
	if (!par_con->root)
		par_con->root = node;
	else
		new_child(par_con->root, node);
}

t_parse_tree	*new_tree(t_ms_token tok)
{
	t_parse_tree	*new;

	new = (t_parse_tree *)malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->tok = tok;
	new->child_nodes = NULL;
	return (new);
}

void	new_child(t_parse_tree *tree, t_parse_tree *sapling)
{
	size_t	count;

	count = 0;
	if (tree->child_nodes)
	{
		while (tree->child_nodes[count])
			count++;
	}
	tree->child_nodes = realloc(tree->child_nodes, sizeof(t_parse_tree *) * (count + 2)); //make ft_realloc
	tree->child_nodes[count] = sapling;
	tree->child_nodes[count + 1] = NULL;
}
