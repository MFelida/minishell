/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:04:40 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/20 16:15:01 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_lex_type
{
	MS_LEX_TOK_ERROR = 0,
	MS_LEX_TOK_ID,
	MS_LEX_TOK_OP,
	MS_LEX_TOK_VAR,
	MS_LEX_TOK_WS,
}	t_lex_type;

typedef enum e_lex_op
{
	MS_LEX_OP_ERROR = 0,
	MS_LEX_OP_INPUT,
	MS_LEX_OP_HEREDOC,
	MS_LEX_OP_OUTPUT,
	MS_LEX_OP_APPEND,
	MS_LEX_OP_PIPE,
}	t_lex_op;

typedef enum e_quote_state
{
	MS_LEX_QUOTE_NONE,
	MS_LEX_QUOTE_SINGLE,
	MS_LEX_QUOTE_DOUBLE,
}	t_quote_state;

typedef struct s_lex_tok
{
	struct s_lex_tok	*next;
	t_lex_type			type;
	t_lex_op			op;
	char				*id;
}	t_lex_tok;

typedef struct s_lex_context
{
	char			*input;
	char			*start;
	char			*curr;
	t_quote_state	quote_state;
	int				error;
}	t_lex_context;

int			is_meta(const char c);
int			is_quote(const char c);
int			is_ws(const char c);
char		*skip_ws(char *s);

t_lex_tok	*lx_default_tok(void);

void		lx_handle_single(t_lex_tok **lex_list, t_lex_context *context);
void		lx_handle_unquoted(t_lex_tok **lex_list, t_lex_context *context);
void		lx_handle_double(t_lex_tok **lex_list, t_lex_context *context);

t_lex_tok	*lx_new_id_tok(t_lex_context *context);
void		lx_handle_meta(t_lex_tok **lex_list, t_lex_context *context);

t_lex_tok	*lex_input(char *input);
t_lex_tok	*expand_vars(t_lex_tok *lex_list);

void		free_lex_list(t_lex_tok **list);
void		free_lex_node(void *node);

#endif
