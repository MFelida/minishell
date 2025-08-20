// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   env_utils.c                                       :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.codam.nl>        +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/20 13:19:40 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 13:57:29 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "hashmap.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>

static void	_swap(char **a, char **b)
{
	char *temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
void	_sort(char **tab)
{
	size_t	i;
	size_t	size;

	if (!tab || !tab[0] || !tab[1])
		return ;
	size = 0;
	while (tab[size++])
		;
	while (size > 1)
	{
		i = 0;
		while (i < size - 1)
		{
			if (ft_strncmp(tab[i], tab[i + 1], _max_len(tab[i], tab[i + 1])) > 0)
				_swap(&tab[i], &tab[i + 1]);
			i++;
		}
		size--;
	}
}

int	_node_to_str(t_hm_node *node, char **dest)
{
	size_t	size;
	
	size = ft_strlen(node->key + node->cap + 3);
	*dest = malloc(size * sizeof(char));
	if (!*dest)
		return (1);
	ft_strlcpy(*dest, node->key, size);
	if (ft_strlen(node->value) > 0)
	{
		ft_strlcat(*dest, "=", size);
		ft_strlcat(*dest, "\"", size);
		ft_strlcat(*dest, node->value, size);
		ft_strlcat(*dest, "\"", size);
	}
	return (0);
}
