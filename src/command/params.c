/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.email.com>       +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/07/22 14:50:55 by mifelida     #+#    #+#                  */
/*   Updated: 2025/10/15 13:53:45 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "libft.h"
#include "parser.h"
#include "redirect.h"

#include <stdlib.h>

t_cmd_params	cmd_params_default(void)
{
	t_cmd_params		res;
	static t_cmd_params	*params_list = NULL;

	res.pid = -1;
	res.envp = NULL;
	res.context = 0;
	res.wstatus = -1;
	res.next = NULL;
	res.redirs = NULL;
	res.head = &params_list;
	res.rusage = (struct rusage){0};
	res.cmd_args = NULL;
	res.bin_path[0] = '\0';
	return (res);
}

static int	_count_chld_nodes(t_parse_node *parent)
{
	int				res;
	t_parse_node	**chld_arr;

	res = 0;
	chld_arr = parent->children;
	while (*(chld_arr++) != NULL)
		res++;
	return (res);
}

static void	_free_cmd_param(void *d)
{
	t_cmd_params	*params;

	params = d;
	params->envp = ft_split_free(params->envp);
	free(params->cmd_args);
	free(d);
}

void	free_cmd_params(t_cmd_params params)
{
	free_parse_tree(params.pt);
	del_redir_list(&params.redirs);
	ft_lstclear((t_list **) params.head, _free_cmd_param);
	params.head = NULL;
}

char	**make_argv(t_parse_node *node)
{
	char	**res;
	int		argc;
	int		i;

	argc = _count_chld_nodes(node);
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
