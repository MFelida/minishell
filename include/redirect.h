/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   redirect.h                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: username <your@mail.com>                    +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 10:07:36 by username     #+#    #+#                  */
/*   Updated: 2025/06/09 10:32:15 by username     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "libft.h"

typedef enum e_redir_type
{
	MS_REDIR_ERROR,
	MS_REDIR_FILE,
	MS_REDIR_FD,
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
	t_redir_src		src;
	t_redir_dest	dest;
	struct s_redir	*next;
}	t_redir;

t_redir	add_redir(t_redir **redir, t_redir_src src, t_redir_dest dest);
int		do_redir(t_redir *redir);

#endif // !REDIRECT_H
