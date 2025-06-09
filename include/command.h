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

# include "fake_parser.h"
# include "redirect.h"

# include <linux/limits.h>
# include <sys/resource.h>

typedef struct s_cmd_params
{
	struct s_cmd_params	*next;
	t_redir				*redirs;
	int					pid;
	char				**cmd_args;
	char				bin_path[PATH_MAX];
	struct rusage		rusage;
	int					wstatus;
	char				**envp;
}	t_cmd_params;

int				cmd_next_node(t_cmd_params params, t_parse_node *node);
t_cmd_params	cmd_params_default(void);

#endif // !COMMAND_H
