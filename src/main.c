/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:25:36 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/29 22:20:53 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fake_parser.h"

#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char *argv[])
{
	if (argc > 1)
		get_parse_tree(argv[1]);
	else
		printf("Hello, World!\n");
	return (EXIT_SUCCESS);
}
