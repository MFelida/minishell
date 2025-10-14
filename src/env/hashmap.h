/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   hashmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+                     */
/*   By: mifelida <mifelida@student.email.com>       +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/08/19 09:35:43 by mifelida     #+#    #+#                  */
/*   Updated: 2025/10/14 16:00:13 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_H
# define HASHMAP_H

# include <stddef.h>
# include <stdint.h>

# define _P		67
# define HM_SIZE	0x10000U

typedef struct s_hm_node
{
	struct s_hm_node	*next;
	const char			*key;
	char				*value;
	size_t				cap;
}	t_hm_node;

typedef struct s_hm
{
	t_hm_node	*data[HM_SIZE];
	size_t		size;
}	t_hm;

int			hm_set_value(t_hm *hm, const char *key, const char *value);
int			hm_unset(t_hm *hm, const char *key);
const char	*hm_get_value(t_hm *hm, const char *key);
void		free_hm(t_hm *hm);

uint16_t	_get_hash(const char *str);;
t_hm_node	*_hm_new_node(const char *key, const char *value);
void		_free_hm_node(void *hm_node);
size_t		_max_len(const char *str1, const char *str2);
int			_safe_strdup(char **dest, const char *src);

#endif	// HASHMAP_H
