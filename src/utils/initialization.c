/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ama <ama@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:17:40 by ama               #+#    #+#             */
/*   Updated: 2025/09/18 12:45:24 by ama              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing_header.h"

void	init_parcon(t_parsing_context *par_con)
{
	par_con->head = NULL;
	par_con->pos = 0;
	par_con->quote = NOT_IN_QUOTES;
	par_con->escaping = NOT_ESCAPING; // redundant for subj and usage?
	par_con->error = 0;
	par_con->tail = NULL;
	par_con->curr = (char *)malloc(sizeof(char) * 3e6);
	par_con->root = NULL;
	par_con->start = par_con->pos;
	if (!par_con->curr)
		exit (1); //make error handling;
}