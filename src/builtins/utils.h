// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   utils.h                                            :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/20 10:54:19 by mifelida     #+#    #+#                  //
/*   Updated: 2025/09/03 15:31:44 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#ifndef UTILS_H
# define UTILS_H

#include "command.h"
typedef	int(*t_builtin_fn)(char **, t_cmd_params *params, ...);

int			_is_option(const char *arg);
const char	*_invalid_option(const char *arg, const char *valid_opts);
int			_set_wstatus(int exit_status, int sig);

#endif	// UTILS_H
