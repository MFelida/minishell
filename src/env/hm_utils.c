// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   hm_utils.c                                        :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/19 11:58:32 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 13:02:28 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "hashmap.h"
#include "libft.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdlib.h>

uint16_t	_get_hash(const char *str)
{
	long long	p;
	uint16_t	res;

	p = 1;
	res = 0;
	while (*str)
	{
		res += p * *str;
		p *= _P;
		str++;
	}
	return (res);
}

t_hm_node	*_hm_new_node(const char *key, const char *value)
{
	t_hm_node	*res;

	res = malloc(sizeof(t_hm_node));
	if (!res)
		return (NULL);
	res->key = ft_strdup(key);
	if (!res->key)
		return (free(res), NULL);
	res->value = ft_strdup(value);
	if (!res->value)
		return (free((char *) res->key), free(res), NULL);
	res->cap = ft_strlen(value) + 1;
	res->next = NULL;
	return (res);
}

void	_free_hm_node(void *hm_node)
{
	t_hm_node	*n;

	if (!hm_node)
		return ;
	n = hm_node;
	if (n->key)
		free((void *) n->key);
	if (n->value)
		free(n->value);
	free(n);
}

size_t	_max_len(const char *str1, const char *str2)
{
	size_t	len;

	len = (ft_strlen(str1));
	if (ft_strlen(str2) > len)
		return (ft_strlen(str2));
	return (len);
}

int	_safe_strdup(char **dest, const char *src)
{
	char *temp;

	temp = ft_strdup(src);
	if (!temp)
		return (1);
	free(*dest);
	*dest = temp;
	return (0);
}
