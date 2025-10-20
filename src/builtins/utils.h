/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/08/20 10:54:19 by mifelida     #+#    #+#                  */
/*   Updated: 2025/09/03 15:31:44 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "command.h"

enum e_builtin_idx
{
	MS_BLTIN_CD,
	MS_BLTIN_ECHO,
	MS_BLTIN_ENV,
	MS_BLTIN_EXIT,
	MS_BLTIN_EXPORT,
	MS_BLTIN_PWD,
	MS_BLTIN_UNSET,
	MS_N_BLTINS,
};

typedef int	(*t_builtin_fn)(char **, t_cmd_params *, ...);

int			_is_option(const char *arg);
const char	*_invalid_option(const char *arg, const char *valid_opts);
int			_set_wstatus(int exit_status, int sig);

#endif	// UTILS_H
