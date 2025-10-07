/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:09:07 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 22:08:02 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_TREE_H
# define PARSE_TREE_H

typedef enum e_ms_ops
{
	MS_OP_CMD = 1,
	MS_OP_BLTIN,
	MS_OP_PIPE,
	MS_OP_FILE_OUTPUT,
	MS_OP_FILE_INPUT,
	MS_OP_FILE_APPEND,
	MS_OP_HEREDOC,
	MS_OP_ERROR = 0,
}	t_ms_ops;

typedef enum e_ms_token_type
{
	MS_TOK_OP = 1,
	MS_TOK_IDENTIFIER,
	MS_TOK_ERROR = 0,
}	t_ms_token_type;

typedef struct s_ms_op_token
{
	t_ms_token_type	type;
	t_ms_ops		op;
	int				value;
}	t_ms_op_token;

typedef struct s_ms_id_token
{
	t_ms_token_type	type;
	char			*value;
}	t_ms_id_token;

typedef union u_ms_token
{
	t_ms_token_type	type;
	t_ms_op_token	op;
	t_ms_id_token	id;
}	t_ms_token;

typedef struct s_parse_node
{
	t_ms_token			tok;
	struct s_parse_node	**children;
}	t_parse_node;

# define MS_CMD_N_CHLD	-1
# define MS_PIPE_N_CHLD	2
# define MS_FO_N_CHLD	2
# define MS_FI_N_CHLD	2

t_parse_node	*get_parse_tree(char *file);
int				count_chld_nodes(t_parse_node *parent);

#endif // !FAKE_PARSER_H
