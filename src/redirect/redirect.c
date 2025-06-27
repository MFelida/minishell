/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: username <your@mail.com>                    +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 10:36:00 by username     #+#    #+#                  */
/*   Updated: 2025/06/25 18:48:39 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"
#include "command.h"
#include "libft.h"
#include "redirect_types.h"
#include "utils.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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
	ft_lstadd_back((t_list **) &cmd->redirs, (t_list *)new);
	return (error);
}

static t_redir_error	_file_to_fd(t_redir_src file_redir, int fd)
{
	int	temp;
	int	err;
	
	temp = open(file_redir.file, file_redir.mode, file_redir.flags);
	if (temp < 0)
		return (MS_REDIR_ERRNO);
	err = dup2(temp, fd);
	close(temp);
	if (err < 0)
		return (MS_REDIR_ERRNO);
	return (MS_REDIR_OK);
}

static t_redir_error	_fd_to_fd(int from, int to)
{
	int	err;

	err = dup2(from, to);
	if (err < 0)
		return (MS_REDIR_ERRNO);
	return (MS_REDIR_OK);
}

void	del_fds_list(void *fds_list)
{
	free(fds_list);
}

void	close_fds(t_open_fds **fds)
{
	t_open_fds	*temp;
	if (!fds)
		return ;
	temp = *fds;
	while (temp)
	{
		close(temp->fd);
		temp = temp->next;
	}
	ft_lstclear((t_list **) fds, del_fds_list);
	free(fds);
}

static t_redir_error	_do_redir(t_redir *redir)
{
	t_redir_error	err;

	err = MS_REDIR_OK;
	if (redir->src.type == MS_REDIR_FILE)
		err |= _file_to_fd(redir->src, redir->dest.fd);
	else if (redir->dest.type == MS_REDIR_FILE)
		err |= _file_to_fd(redir->dest, redir->src.fd);
	else
		err |= _fd_to_fd(redir->src.fd, redir->dest.fd);
	return (err);
}

t_redir_error	do_redirs(t_cmd_params *params)
{
	t_redir			*redirs;
	t_redir_error	err;

	redirs = params->redirs;
	while (redirs)
	{
		err = _do_redir(redirs);
		if (err)
			return (err);
		redirs = redirs->next;
	}
	return (MS_REDIR_OK);
}
