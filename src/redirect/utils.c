/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:45:43 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 14:49:36 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "redirect.h"

t_redir	*_new_redir(void)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->next = NULL;
	return (new);
}

t_redir_error	_get_src_dest(t_redir_src *d, t_redir_src	*r)
{
	if (!r)
		return (MS_REDIR_EMPTY);
	if (r->type == MS_REDIR_FD && r->fd < 0)
		return (MS_REDIR_INVALID_FD);
	if (r->type == MS_REDIR_FILE && r->file == NULL)
		return (MS_REDIR_NULL_FILE);
	*d = *r;
	return (MS_REDIR_OK);
}


