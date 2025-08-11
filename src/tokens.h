/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:24:31 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/11 14:00:20 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_ms_operators
{
	MS_OP_ERROR = 0,
	MS_FILE_OUTPUT,
	MS_FILE_APPEND,
	MS_FILE_INPUT,
	MS_HEREDOC,
	MS_PIPE,
	MS_ENV_VAR,
	MS_CMD,
}	t_ms_operators;

typedef enum e_ms_token_type
{
	MS_TOK_ERROR = 0,
	MS_TOK_OPERATOR,
	MS_TOK_IDENTIFIER,
	MS_TOK_COMMAND,
}	t_ms_token_type;

typedef enum e_ms_cmd_type
{
	MS_CMD_ERROR = 0,
	MS_CMD_ECHO,
	MS_CMD_CD,
	MS_CMD_PWD,
	MS_CMD_EXPORT,
	MS_CMD_UNSET,
	MS_CMD_ENC,
	MS_CMD_EXIT,
	MS_CMD_BIN,
}	t_ms_cmd_type;

typedef struct s_ms_op_token
{
	t_ms_token_type	type;
	t_ms_operators	op;
	int				value;
}	t_ms_op_token;

typedef struct s_ms_id_token
{
	t_ms_token_type	type;
	char			*value;
}	t_ms_id_token;

typedef struct s_ms_cmd_token
{
	t_ms_token_type	type;
	t_ms_cmd_type	cmd;
}	t_ms_cmd_token;

typedef union s_ms_token
{
	t_ms_token_type	type;
	t_ms_op_token	op;
	t_ms_id_token	id;
	t_ms_cmd_type	cmd;
}	t_ms_token;

typedef struct s_parse_tree
{
	t_ms_token			tok;
	struct s_parse_tree	**child_nodes;
}	t_parse_tree;
