// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   hashmap.c                                         :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/18 11:31:22 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 18:00:53 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "hashmap.h"
#include "libft.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void	free_hm(t_hm *hm)
{
	size_t	i;

	i = 0;
	while (i < HM_SIZE)
	{
		ft_lstclear((t_list **) &hm->data[i], _free_hm_node);
		i++;
	}
}

int	hm_set_value(t_hm *hm, const char *key, const char *value)
{
	t_hm_node	**head;
	t_hm_node	*node;

	head = &hm->data[_get_hash(key)];
	node = *head;
	while (node && ft_strncmp(key, node->key, _max_len(key, node->key)))
		node = node->next;
	if (value == NULL)
		value = "";
	if (!node)
	{
		ft_lstadd_front((t_list **) head, (t_list *) _hm_new_node(key, value));
		hm->size++;
		return (*head && ft_strncmp(key, (*head)->key, _max_len(key, (*head)->key)));
	}
	if (node->cap > ft_strlen(value))
		ft_strlcpy(node->value, value, ft_strlen(value) + 1);
	else
	{
		if (_safe_strdup(&node->value, value))
			return (1);
		node->cap = ft_strlen(value) + 1;
	}
	return (0);
}

int	hm_unset(t_hm *hm, const char *key)
{
	uint16_t	hash;
	t_hm_node	*prev;
	t_hm_node	*node;

	prev = NULL;
	hash = _get_hash(key);
	node = hm->data[hash];
	while (node && ft_strncmp(key, node->key, _max_len(key, node->key)))
	{
		prev = node;
		node = node->next;
	}
	if (!node)
		return (1);
	if (!prev)
		(*hm).data[hash] = node->next;
	else
		prev->next = node->next;
	_free_hm_node(node);
	hm->size--;
	return (0);
}

const char	*hm_get_value(t_hm *hm, const char *key)
{
	t_hm_node	*node;

	node = hm->data[_get_hash(key)];
	if (!node)
		return (NULL);
	while (node && ft_strncmp(key, node->key, _max_len(key, node->key)))
		node = node->next;
	if (node)
		return (node->value);
	return (NULL);
}
