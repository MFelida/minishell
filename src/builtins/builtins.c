// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
/*   builtins.c                                         :+:      :+:    :+:   */
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 14:36:02 by mifelida     #+#    #+#                  //
/*   Updated: 2025/08/29 15:27:39 by mifelida         ###   ########.fr       */
//                                                                            //
// ************************************************************************** //

#include "builtins.h"
#include "command.h"
#include "exit_statuses.h"
#include "libft.h"
#include "redirect.h"
#include "utils.h"

#include <stddef.h>

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

static const char*	g_builtin_names[] =
{
	[MS_BLTIN_CD] = "cd",
	[MS_BLTIN_ECHO] = "echo",
	[MS_BLTIN_ENV] = "env",
	[MS_BLTIN_EXIT] = "exit",
	[MS_BLTIN_PWD] = "pwd",
	[MS_BLTIN_EXPORT] = "export",
	[MS_BLTIN_UNSET] = "unset",
};

int	ms_cd(char **args, t_cmd_params *params ,...);
int	ms_echo(char **args, t_cmd_params *params, ...);
int	ms_env(char **args, t_cmd_params *params, ...);
int	ms_exit(char **args, t_cmd_params *params, ...);
int	ms_pwd(char **args, t_cmd_params *params, ...);
int	ms_export(char **args, t_cmd_params *params, ...);
int	ms_unset(char **args, t_cmd_params *params, ...);

static const t_builtin_fn	g_builtins[] =
{
	[MS_BLTIN_CD] = ms_cd,
	[MS_BLTIN_ECHO] = ms_echo,
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

int	do_builtin(const char *builtin, t_cmd_params *params)
{
	t_builtin_fn	builtin_fn;
	int				*stdio;
	int				res;

	builtin_fn = _get_builtin_fn(builtin);
	if (!builtin_fn)
	{
		ft_print_err("Not a valid builtin", 2, "minishell", params->cmd_args[0]);
		return (MS_FAILURE);
	}
	stdio = ms_save_stdio();
	do_redirs(params);
	res = builtin_fn(params->cmd_args, params);
	ms_restore_stdio(stdio);
	return (res);
}
