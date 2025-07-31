/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_parser.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:09:07 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 18:13:23 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAKE_PARSER_H
# define FAKE_PARSER_H

typedef enum e_fp_ops
{
	FP_OP_CMD = 1,
	FP_OP_PIPE,
	FP_OP_FILE_OUTPUT,
	FP_OP_FILE_INPUT,
	FP_OP_FILE_APPEND,
	FP_OP_ERROR = 0,
}	t_fp_ops;

typedef enum e_fp_token_type
{
	FP_TOK_OP = 1,
	FP_TOK_IDENTIFIER,
	FP_TOK_ERROR = 0,
}	t_fp_token_type;

typedef struct s_fp_op_token
{
	t_fp_token_type	type;
	t_fp_ops		op;
}	t_fp_op_token;

typedef struct s_fp_id_token
{
	t_fp_token_type	type;
	char			*value;
}	t_fp_id_token;

typedef union u_fp_token
{
	t_fp_token_type	type;
	t_fp_op_token	op;
	t_fp_id_token	id;
}	t_fp_token;

typedef struct s_parse_node
{
	t_fp_token			tok;
	struct s_parse_node	**children;
}	t_parse_node;

# define FP_CMD_N_CHLD	-1
# define FP_PIPE_N_CHLD	2
# define FP_FO_N_CHLD	2
# define FP_FI_N_CHLD	2

t_parse_node	*get_parse_tree(char *file);
int				count_chld_nodes(t_parse_node *parent);

#endif // !FAKE_PARSER_H
