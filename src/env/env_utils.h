// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   env_utils.h                                       :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/20 13:35:31 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 13:57:54 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

#include "hashmap.h"

void	_sort(char **tab);
char	*_node_to_str(t_hm_node *node, char **dest, int unquoted);

#endif	// ENV_UTILS_H
