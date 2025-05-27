/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_format.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:53:35 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/27 18:21:56 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_FORMAT_H
# define FILE_FORMAT_H

# include "fake-parser/fake_parser.h"

char	*g_op_strings[] = {
[FP_OP_CMD] = "cmd",
[FP_OP_PIPE] = "pipe",
[FP_OP_FILE_OUTPUT] = "output",
[FP_OP_FILE_INPUT] = "input",
[FP_OP_ERROR] = "",
};

#endif // !FILE_FORMAT_H
