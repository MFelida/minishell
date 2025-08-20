// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   builtins.c                                        :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:36:02 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 10:57:28 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "builtins.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>

enum e_builtin_idx
{
	MS_BLTIN_CD,
	MS_BLTIN_ENV,
	MS_BLTIN_EXIT,
	MS_BLTIN_EXPORT,
	MS_BLTIN_PWD,
	MS_BLTIN_UNSET,
	MS_N_BLTINS,
};

static const char*	g_builtin_names[] =
{
	[MS_BLTIN_CD] = "cd",
	[MS_BLTIN_ENV] = "env",
	[MS_BLTIN_EXIT] = "exit",
	[MS_BLTIN_PWD] = "pwd",
	[MS_BLTIN_EXPORT] = "export",
	[MS_BLTIN_UNSET] = "unset",
};

int	ms_cd(char **args);
int	ms_env(char **args);
int	ms_exit(char **args);
int	ms_pwd(char **args);
int	ms_export(char **args);
int	ms_unset(char **args);

static const t_builtin_fn	g_builtins[] =
{
	[MS_BLTIN_CD] = ms_cd,
	[MS_BLTIN_ENV] = ms_env,
	[MS_BLTIN_EXIT] = ms_exit,
	[MS_BLTIN_EXPORT] = ms_export,
	[MS_BLTIN_PWD] = ms_pwd,
	[MS_BLTIN_UNSET] = ms_unset,
};

static t_builtin_fn	_get_builtin_fn(const char *str)
{
	size_t	i;

	i = 0;
	while (i < MS_N_BLTINS && ft_strncmp(str, g_builtin_names[i], 100))
		i++;
	if (i >= MS_N_BLTINS)
		return (NULL);
	return (g_builtins[i]);
}

int	do_builtin(const char *builtin, char **args)
{
	t_builtin_fn	builtin_fn;

	builtin_fn = _get_builtin_fn(builtin);
	if (!builtin_fn)
		return (-1);
	return (builtin_fn(args));
}
