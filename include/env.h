// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   env.h                                             :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/18 10:35:44 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/18 12:11:41 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#ifndef ENV_H
# define ENV_H

#include <stddef.h>
#include <stdint.h>

typedef struct s_hm_node
{
	struct s_hm_node	*next;
	const char			*key;
	char				*value;
	size_t				cap;
}	t_hm_node;

typedef t_hm_node*	t_env[UINT16_MAX];

t_hm_node	*hm_node_new(const char *key, char *value);
void		hm_free(t_hm_node	**head);

#endif	// ENV_H
