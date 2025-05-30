/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:04:42 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/30 16:10:52 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

char	*find_bin(const char* name)
{
	static char	*bin_path;
	char		*path_var;

	*bin_path = 0;
	path_var = getenv("PATH");
	printf("%s\n%s\v", path_var, name);
	return (bin_path);
}
