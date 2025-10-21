/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:54 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 16:32:02 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "parse_tree.h"
#include "libft.h"

static void	_indent(int tab_depth)
{
	while (tab_depth-- > 0)
		ft_putstr_fd("  ", 1);
}

void	fp_print(t_parse_node node, int tab_depth)
{
	if (node.tok.op.op == MS_OP_PIPE)
	{
		_indent(tab_depth);
		printf("pipe:\n");
		fp_print(*node.children[0], tab_depth + 1);
		fp_print(*node.children[1], tab_depth + 1);
	}
	if (node.tok.op.op == MS_OP_CMD || node.tok.op.op == MS_OP_BLTIN)
	{
		_indent(tab_depth);
		printf("cmd:\n");
		while (*(node.children))
		{
			_indent(tab_depth + 1);
			printf("%s\n", (*node.children++)->tok.id.value);
		}
	}
	if (node.tok.op.op == MS_OP_FILE_INPUT)
	{
		_indent(tab_depth);
		printf("input:\n");
		_indent(tab_depth + 1);
		printf("%s\n", node.children[0]->tok.id.value);
		fp_print(*node.children[1], tab_depth + 1);
	}
	if (node.tok.op.op == MS_OP_FILE_OUTPUT)
	{
		_indent(tab_depth);
		printf("output:\n");
		_indent(tab_depth + 1);
		printf("%s\n", node.children[0]->tok.id.value);
		fp_print(*node.children[1], tab_depth + 1);
	}
	if (node.tok.op.op == MS_OP_FILE_APPEND)
	{
		_indent(tab_depth);
		printf("append:\n");
		_indent(tab_depth + 1);
		printf("%s\n", node.children[0]->tok.id.value);
		fp_print(*node.children[1], tab_depth + 1);
	}
	if (node.tok.op.op == MS_OP_HEREDOC)
	{
		_indent(tab_depth);
		printf("heredoc:\n");
		_indent(tab_depth + 1);
		printf("fd: %d, word: %s\n", node.tok.op.value, node.children[0]->tok.id.value);
		fp_print(*node.children[1], tab_depth + 1);
	}
}
