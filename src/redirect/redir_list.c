/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:42:31 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/14 14:44:53 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "libft.h"
#include "redirect_types.h"

#include <stdlib.h>

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

t_redir_error	add_redir(t_cmd_params *cmd, t_redir_src src, t_redir_dest dest)
{
	t_redir			*new;
	t_redir_error	error;

	error = MS_REDIR_OK;
	new = _new_redir();
	if (!new)
		return (MS_REDIR_ERRNO);
	error |= _get_src_dest(&new->src, &src);
	error |= _get_src_dest(&new->dest, &dest);
	ft_lstadd_back((t_list **) &cmd->redirs, (t_list *) new);
	return (error);
}

void	del_redir_list(t_redir **redir)
{
	ft_lstclear((t_list **) redir, free);
}
