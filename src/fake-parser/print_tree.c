/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:54 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/30 10:53:16 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "fake_parser.h"
#include "libft.h"

void	_indent(int tab_depth)
{
	while (tab_depth-- > 0)
		ft_putstr_fd("  ", 1);
}

void	fp_print(t_parse_node node, int tab_depth)
{
	if (node.tok.op.op == FP_OP_PIPE)
	{
		_indent(tab_depth);
		printf("pipe:\n");
		fp_print(*node.children[0], tab_depth + 1);
		fp_print(*node.children[1], tab_depth + 1);
	}
	if (node.tok.op.op == FP_OP_CMD)
	{
		_indent(tab_depth);
		printf("cmd:\n");
		while (*(node.children))
		{
			_indent(tab_depth + 1);
			printf("%s\n", (*node.children++)->tok.id.value);
		}
	}
}
