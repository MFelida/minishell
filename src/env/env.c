/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.email.com>       +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/08/19 10:42:11 by mifelida     #+#    #+#                  */
/*   Updated: 2025/10/20 17:20:03 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_utils.h"
#include "hashmap.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static t_hm	g_env_hm = {0};

void	free_env(void)
{
	free_hm(&g_env_hm);
}

const char	*ms_getenv(const char *key)
{
	if (!key)
		return (NULL);
	if (!ft_strncmp("?", key, 2))
		return (_exit_status(NULL));
	if (!ft_strncmp("$", key, 2))
		return ("$");
	if (!valid_envvar(key))
		return (NULL);
	return (hm_get_value(&g_env_hm, key));
}

int	ms_setenv(const char *key, const char *value)
{
	if (!key)
		return (1);
	if (!ft_strncmp("?", key, 2))
		return (!!_exit_status(value));
	if (!valid_envvar(key))
		return (1);
	return (hm_set_value(&g_env_hm, key, value));
}

int	ms_unsetenv(const char *key)
{
	return (hm_unset(&g_env_hm, key));
}

char	**ms_getenv_full(int sorted, int inc_empty, int unquoted)
{
	char		**res;
	char		**temp;
	t_hm_node	*node;
	size_t		i;

	res = ft_calloc(g_env_hm.size + 1, sizeof(char *));
	if (!res)
		return (NULL);
	temp = res;
	i = 0;
	while (i < HM_SIZE)
	{
		node = g_env_hm.data[i++];
		while (node)
		{
			if (valid_envvar(node->key)
				&& (inc_empty || ft_strlen(node->value) > 0))
				if (_node_to_str(node, &*temp++, unquoted))
					return (ft_split_free(res), (NULL));
			node = node->next;
		}
	}
	if (sorted)
		_sort(res);
	return (res);
}
