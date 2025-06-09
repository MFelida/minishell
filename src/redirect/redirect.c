/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirect.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: username <your@mail.com>                    +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 10:36:00 by username     #+#    #+#                  */
/*   Updated: 2025/06/09 11:32:26 by username     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"
#include "libft.h"

#include <stdlib.h>

static t_redir	*_new_redir()
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->src.type = MS_REDIR_NOTUSED;
	new->dest.type = MS_REDIR_NOTUSED;
	new->next = NULL;
	return (new);
}

static t_redir_error	_get_src_dest(t_redir_src *d, t_redir_src	*r)
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

t_redir_error	add_redir(t_redir **redir, t_redir_src *src, t_redir_dest *dest)
{
	t_redir			*new;
	t_redir_error	error;

	if (!redir)
		return (MS_REDIR_NULL_LIST);
	error = MS_REDIR_OK;
	new = _new_redir();
	if (!new)
		return (MS_REDIR_ERRNO);
	if (src)
		error |= _get_src_dest(&new->src, src);
	if (dest)
		error |= _get_src_dest(&new->dest, dest);
	ft_lstadd_back((t_list **)redir, (t_list *)new);
	return (error);
}

int	do_redir(t_redir *redir);
