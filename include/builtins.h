// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   builtins.h                                        :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:29:59 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 10:55:48 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#ifndef BUILTINS_H
# define BUILTINS_H

#include "command.h"
int	do_builtin(const char *builtin, t_cmd_params params);

#endif	// BUILTINS_H
