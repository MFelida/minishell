/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:40:49 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 14:56:13 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "command.h"
# include "redirect_types.h"

t_redir_error	add_redir(t_cmd_params *cmd, t_redir_src from, t_redir_dest to);
t_open_fds		*new_fd(const int	fd);
t_redir_error	do_redirs(t_cmd_params *params);
void			close_fds(void);
void			del_redir_list(t_redir	**redirs);
int				ms_pipe(t_pipe *fds);

#endif // !REDIRECT_H
