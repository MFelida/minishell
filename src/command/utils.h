// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   utils.h                                            :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/07/22 14:53:29 by mifelida     #+#    #+#                  //
/*   Updated: 2025/09/23 11:19:24 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#ifndef UTILS_H
# define UTILS_H

#include "command.h"

int		find_bin(char *dest, const char *name);
char	**make_argv(t_parse_node *node);

#endif
