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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static t_open_fds	*g_open_fds = {0};

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

t_open_fds	*new_fd(const int	fd)
{
	t_open_fds	*new;

	new = malloc(sizeof(t_open_fds));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->next = NULL;
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

int	*ms_save_stdio(void)
{
	int	*fds;
	int	i;

	fds = ft_calloc(4, sizeof(int));
	if (!fds)
		return (NULL);
	i = 0;
	while (i < 3)
	{
		fds[i] = dup(i);
		i++;
	}
	return (fds);
}

int	ms_restore_stdio(int *fds)
{
	int	i;
	int	res;

	res = 0;
	i = 0;
	while (fds[i])
	{
		res |= dup2(fds[i], i) == i;
		close(fds[i]);
		i++;
	}
	free(fds);
	return (res);
}

int	ms_pipe(t_pipe *fds)
{
	if (pipe(fds->a))
		return (-1);
	ft_lstadd_back((t_list **) &g_open_fds, (t_list *) new_fd(fds->read));
	ft_lstadd_back((t_list **) &g_open_fds, (t_list *) new_fd(fds->write));
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
	del_redir_list(&params->redirs);
	return (MS_REDIR_OK);
}
