/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirect.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: username <your@mail.com>                    +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 10:07:36 by username     #+#    #+#                  */
/*   Updated: 2025/06/09 11:49:21 by username     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

typedef enum e_redir_error
{
	MS_REDIR_OK			= 0x00,
	MS_REDIR_NULL_LIST	= 0x01 << 0,
	MS_REDIR_NULL_FILE	= 0x01 << 1,
	MS_REDIR_EMPTY		= 0x01 << 2,
	MS_REDIR_ERRNO		= 0x01 << 3,
	MS_REDIR_INVALID_FD	= 0x01 << 4,
}	t_redir_error;

typedef enum e_redir_type
{
	MS_REDIR_ERROR,
	MS_REDIR_FILE,
	MS_REDIR_FD,
	MS_REDIR_NOTUSED,
	MS_REDIR_N_TYPES,
}	t_redir_type;

typedef struct s_redir_src
{
	t_redir_type	type;
	union
	{
		char		*file;
		int			fd;
	};
}	t_redir_src;

typedef t_redir_src	t_redir_dest;

typedef struct s_redir
{
	struct s_redir	*next;
	t_redir_src		src;
	t_redir_dest	dest;
}	t_redir;

t_redir_error	add_redir(
					t_redir **redir,
					t_redir_src *src,
					t_redir_dest *dest
					);
int				do_redir(t_redir *redir);

#endif // !REDIRECT_H
