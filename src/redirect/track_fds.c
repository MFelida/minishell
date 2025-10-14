/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:50:22 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/14 13:02:44 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include "redirect_types.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static t_open_fds	*g_open_fds = {0};

t_open_fds	*new_fd(const int	fd)
{
	t_open_fds	*new;

	new = malloc(sizeof(t_open_fds));
	if (!new)
	{
		ft_print_err(strerror(errno), 2, "minishell", __FUNCTION__);
		return (NULL);
	}
	new->fd = fd;
	new->next = NULL;
	ft_lstadd_back((t_list **) &g_open_fds, (t_list *)new);
	return (new);
}

static void	_del_open_fds(void *node)
{
	close(((t_open_fds *) node)->fd);
	free(node);
}

void	close_fds(void)
{
	ft_lstclear((t_list **) &g_open_fds, _del_open_fds);
}

