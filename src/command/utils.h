/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.email.com>       +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/22 14:53:29 by mifelida     #+#    #+#                  */
/*   Updated: 2025/10/15 13:52:36 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "command.h"
# include "parse_tree.h"

_Noreturn void	cmd_exec(t_cmd_params params);
int				find_bin(char *dest, const char *name);
int				is_rel_path(const char *str);
char			**make_argv(t_parse_node *node);

int				cmd_input_redir(t_cmd_params params, t_parse_node *node);
int				cmd_hear_doc(t_cmd_params params, t_parse_node *node);
int				cmd_output_redir(t_cmd_params params, t_parse_node *node);
int				cmd_output_append(t_cmd_params params, t_parse_node *node);

#endif
