/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_types.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:25:23 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 13:59:03 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_TYPES_H
# define REDIRECT_TYPES_H

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
		int			fd;
		struct
		{
			char	*file;
			int		mode;
			int		flags;
		};
	};
}	t_redir_src;

typedef t_redir_src	t_redir_dest;

typedef struct s_redir
{
	struct s_redir	*next;
	t_redir_src		src;
	t_redir_dest	dest;
}	t_redir;

typedef struct s_open_fds
{
	struct s_open_fds	*next;
	int					fd;
}	t_open_fds;

typedef union u_pipe
{
	int		a[2];
	struct
	{
		int	read;
		int	write;
	};
}	t_pipe;

#endif // !REDIRECT_TYPES_H
