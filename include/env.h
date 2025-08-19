// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   env.h                                             :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/18 10:35:44 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/19 12:54:07 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#ifndef ENV_H
# define ENV_H

#include <stddef.h>
#include <stdint.h>

int			init_env(void);
void		free_env(void);
const char	*ms_getenv(const char *key);
int			ms_setenv(const char *key, const char *value);
int			ms_unset(const char *key);

#endif	// ENV_H
