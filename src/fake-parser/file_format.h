/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_format.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:53:35 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 18:14:13 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_FORMAT_H
# define FILE_FORMAT_H

# include "fake_parser.h"

char	*g_op_strings[] = {
[FP_OP_CMD] = "cmd",
[FP_OP_PIPE] = "pipe",
[FP_OP_FILE_OUTPUT] = "output",
[FP_OP_FILE_INPUT] = "input",
[FP_OP_ERROR] = "error",
};

t_parse_node	*get_pipe_node(int fd);
t_parse_node	*get_cmd_node(int fd);

typedef t_parse_node	*(*t_node_getter)(int);

t_node_getter	g_node_getters[] = {
[FP_OP_CMD] = get_cmd_node,
[FP_OP_PIPE] = get_pipe_node,
};

#endif // !FILE_FORMAT_H
