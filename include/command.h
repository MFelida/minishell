/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:19 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/02 16:31:50 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

#include <linux/limits.h>
# include <sys/resource.h>
# include "fake_parser.h"

typedef enum e_cmd_io_type
{
	MS_CMD_IO_FILE = 1,
	MS_CMD_IO_FD,
	MS_CMD_IO_ERROR = 0,
}	t_cmd_io_type;

typedef struct s_cmd_io
{
	t_cmd_io_type	type;
	union
	{
		int		fd;
		struct
		{
			char	*name;
			int		flags;
		}	file;
	};
}	t_cmd_io;

typedef struct s_cmd_params
{
	t_cmd_io		stdin;
	t_cmd_io		stdout;
	t_cmd_io		stderr;
	int				pid;
	char			**cmd_args;
	char			bin_path[PATH_MAX];
	struct rusage	rusage;
	int				wstatus;
	char			**envp;
}	t_cmd_params;

int	cmd_next_node(t_cmd_params params, t_parse_node *node);
t_cmd_params	cmd_params_default(void);

#endif // !COMMAND_H
