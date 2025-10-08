/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:32:19 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 23:29:54 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "redirect_types.h"
# include "parse_tree.h"

# include <linux/limits.h>
# include <sys/resource.h>

// Errors used to indicate command ran abnormally
enum e_cmd_error
{
	MS_CMD_ERROR_OK = 0,
	MS_CMD_ERROR_FAILURE = 0x01 << 1,
	MS_CMD_ERROR_PIPE = 0x01 << 2,
	MS_CMD_ERROR_MALLOC = 0x01 << 3,
	MS_CMD_ERROR_SHOULD_EXIT = 0x01 << 4,
};

enum e_cmd_context
{
	MS_CMD_CONTEXT_INVALID = 0,
	MS_CMD_CONTEXT_SIMPLE = 0x01 << 0,
	MS_CMD_CONTEXT_COMPOUND = 0x01 << 1,
	MS_CMD_CONTEXT_PIPE = 0x01 << 2,
	MS_CMD_CONTEXT_BLTIN = 0x01 << 3,
};

typedef struct s_cmd_params
{
	struct s_cmd_params	*next;
	struct s_cmd_params	**head;
	t_redir				*redirs;
	int					pid;
	char				**cmd_args;
	char				bin_path[PATH_MAX];
	enum e_cmd_context	context;
	struct rusage		rusage;
	int					wstatus;
	char				**envp;
	t_parse_node		**pt;
}	t_cmd_params;

int				cmd_next_node(t_cmd_params *params, t_parse_node *node);
t_cmd_params	cmd_params_default(void);
void			free_cmd_params(t_cmd_params params);

#endif // !COMMAND_H
