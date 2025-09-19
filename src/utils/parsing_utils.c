/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:58:39 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/19 10:41:49 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

void	trim_space_nodes(t_parsing_context *par_con)
{
	t_token_list	*cur;
	t_token_list	*prev;
	t_token_list	*to_free;

	prev = NULL;
	cur = par_con->head;
	while(cur)
	{
		if (ft_isspace(cur->string[0]))
		{
			if (prev)
				prev->next = cur->next;
			else
				par_con->head = cur->next;
			to_free = cur;
			cur = cur->next;
			space_node_free(to_free);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

void	space_node_free(t_token_list *to_free)
{
	free(to_free->string);
	free(to_free->type);
	free(to_free);
}