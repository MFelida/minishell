/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 07:24:19 by ama               #+#    #+#             */
/*   Updated: 2025/09/18 09:20:25 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

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