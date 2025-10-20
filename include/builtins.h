/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/08/19 14:29:59 by mifelida     #+#    #+#                  */
/*   Updated: 2025/10/20 16:30:42 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "command.h"

int	is_builtin(const char *name);
int	do_builtin(const char *builtin, t_cmd_params *params);

#endif	// BUILTINS_H
