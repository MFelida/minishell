// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   env.h                                              :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/18 10:35:44 by mifelida     #+#    #+#                  //
/*   Updated: 2025/09/03 14:31:48 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#ifndef ENV_H
# define ENV_H

#include <stddef.h>
#include <stdint.h>

int			valid_pwd(const char *env_pwd);
int			init_env(void);
void		free_env(void);
int			valid_envvar(const char *var);
const char	*ms_getenv(const char *key);
int			ms_setenv(const char *key, const char *value);
int			ms_set_exitstatus(const int status);
int			ms_unsetenv(const char *key);
char		**ms_getenv_full(int sorted, int inc_empty, int unquoted);
int			print_env(int sorted, int inc_empty);

enum e_print_env_options
{
	MS_ENV_PRINT_UNSORTED = 0,
	MS_ENV_PRINT_SORTED = 1,
	MS_ENV_PRINT_EXCL_EMPTY = 0,
	MS_ENV_PRINT_INCL_EMPTY = 1,
};

#endif	// ENV_H
