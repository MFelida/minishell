/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: username <your@mail.com>                    +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 10:36:00 by username     #+#    #+#                  */
/*   Updated: 2025/10/14 14:46:05 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect.h"
#include "command.h"
#include "libft.h"
#include "redirect_types.h"
#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static t_redir_error	_file_to_fd(t_redir_src file_redir, int fd)
{
	int	temp;
	int	err;

	temp = open(file_redir.file, file_redir.mode, file_redir.flags);
	if (temp < 0)
	{
		ft_print_err(strerror(errno), 2, "minishell", file_redir.file);
		return (MS_REDIR_ERRNO);
	}
	err = dup2(temp, fd);
	close(temp);
	if (err < 0)
	{
		ft_print_err(strerror(errno), 1, "minishell");
		return (MS_REDIR_ERRNO);
	}
	return (MS_REDIR_OK);
}

static t_redir_error	_fd_to_fd(int from, int to)
{
	int	err;

	err = dup2(from, to);
	if (err < 0)
	{
		ft_print_err(strerror(errno), 1, "minishell");
		return (MS_REDIR_ERRNO);
	}
	return (MS_REDIR_OK);
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

int	ms_pipe(t_pipe *fds)
{
	if (pipe(fds->a))
	{
		ft_print_err(strerror(errno), 2, "minishell", "pipe");
		return (-1);
	}
	if (!new_fd(fds->read) || !new_fd(fds->write))
		return (-1);
	return (0);
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
