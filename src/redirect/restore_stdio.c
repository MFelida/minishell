/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_stdio.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:49:07 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/14 12:49:49 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int	*ms_save_stdio(void)
{
	int	*fds;
	int	i;

	fds = ft_calloc(4, sizeof(int));
	if (!fds)
		return (NULL);
	i = 0;
	while (i < 3)
	{
		fds[i] = dup(i);
		if (fds[i] < 0)
		{
			ft_print_err(strerror(errno), 2, "minishell", "dup");
			while (--i >= 0)
				close(fds[i]);
			free(fds);
			return (NULL);
		}
		i++;
	}
	return (fds);
}

int	ms_restore_stdio(int *fds)
{
	int	i;
	int	res;

	if (!fds)
		return (0);
	res = 0;
	i = 0;
	while (fds[i])
	{
		res |= dup2(fds[i], i) == i;
		close(fds[i]);
		i++;
	}
	free(fds);
	return (res);
}

void	ms_close_stdio(void)
{
	int i;

	i = 0;
	while (i < 3)
		close(i++);
}

