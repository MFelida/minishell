// ************************************************************************** //
//                                                                            //
//                                                       ::::::::             //
//   print_env.c                                       :+:    :+:             //
//                                                    +:+                     //
//   By: mifelida <mifelida@student.email.com>       +#+                      //
//                                                  +#+                       //
//   Created: 2025/08/20 17:50:14 by mifelida     #+#    #+#                  //
//   Updated: 2025/08/20 17:54:53 by mifelida     ########   odam.nl          //
//                                                                            //
// ************************************************************************** //

#include "env.h"
#include "libft.h"

#include <stddef.h>
#include <stdio.h>

int	print_env(int sorted, int inc_empty)
{
	char	**tab;
	size_t	i;

	tab = ms_getenv_full(sorted, inc_empty);
	if (!tab)
		return (1);
	i = 0;
	while (tab[i])
		printf("%s\n", tab[i++]);
	ft_split_free(tab);
	return (0);
}
