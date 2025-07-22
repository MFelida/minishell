// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   params.c                                          :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/07/22 14:50:55 by mifelida     #+#    #+#                  //
//   Updated: 2025/07/22 15:42:35 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "command.h"
#include "libft.h"

#include <stdlib.h>

t_cmd_params	cmd_params_default(void)
{
	t_cmd_params	res;
	extern char		**environ;

	res.pid = -1;
	res.envp = environ;
	res.wstatus = -1;
	res.next = NULL;
	res.redirs = NULL;
	res.open_fds = malloc(sizeof(t_open_fds *));
	*res.open_fds = NULL;
	res.head = NULL;
	res.rusage = (struct rusage){0};
	res.cmd_args = NULL;
	res.bin_path[0] = '\0';
	return (res);
}

static void	_free_cmd_param(void *d)
{
	t_cmd_params	*params;

	params = d;
	free(params->cmd_args);
	free(d);
}

void	free_cmd_params(t_cmd_params params)
{
	ft_lstclear((t_list **) params.head, _free_cmd_param);
}

char	**make_argv(t_parse_node *node)
{
	char	**res;
	int		argc;
	int		i;

	argc = count_chld_nodes(node);
	res = ft_calloc((argc + 1), sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (node->children[i])
	{
		res[i] = node->children[i]->tok.id.value;
		i++;
	}
	return (res);
}
