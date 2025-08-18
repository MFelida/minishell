// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   hashmap.c                                         :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/18 11:31:22 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/19 09:46:10 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "env.h"
#include "libft.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define _P		67
#define HM_SIZE	0x10000U

static	t_env	env_hm;

static uint16_t	_get_hash(const char *str)
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

static size_t	_max_len(const char *str1, const char *str2)
{
	size_t	len;

	len = (ft_strlen(str1));
	if (ft_strlen(str2) > len)
		return (ft_strlen(str2));
	return (len);
}

int	hm_set_value(const char *key, char *value)
{
	uint16_t	hash;
	t_hm_node	*node;
	char		*temp;

	hash = _get_hash(key);
	node = (*map)[hash];
	if (!node)
	{
		node = hm_node_new(key, value);
		return (1);
	}
	while (node && ft_strncmp(key, node->key, _max_len(key, node->key)))
		node = node->next;
	if (node->cap >= ft_strlen(value) + 1)
		ft_strlcpy(node->value, value, ft_strlen(value) + 1);
	else
	{
		temp = ft_strdup(value);
		if (!temp)
			return (1);
		free(node->value);
		node->value = temp;
		node->cap = ft_strlen(value) + 1;
	}
	return (0);
}

char	*hm_get_value(const char *key)
{
	uint16_t	hash;
	t_hm_node	*node;

	hash = _get_hash(key);
	node = (*map)[hash];
	if (!node)
		return (NULL);
	while (node)
	{
		if (!ft_strncmp(key, node->key, _max_len(key, node->key)))
			return (node->value);
		node = node->next;
	}
	return (NULL);
}
