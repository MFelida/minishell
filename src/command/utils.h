// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   utils.h                                           :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/07/22 14:53:29 by mifelida     #+#    #+#                  //
//   Updated: 2025/07/22 15:35:33 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#ifndef UTILS_H
# define UTILS_H

#include "command.h"

int		find_bin(char *dest, const char *name);
void	free_cmd_params(t_cmd_params params);
char	**make_argv(t_parse_node *node);

#endif
