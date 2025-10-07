/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_strerror.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 22:13:17 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 22:36:52 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static const char	*g_op_err_str[] = {
	"error",
	[MS_LEX_OP_APPEND] = "`>>'",
	[MS_LEX_OP_OUTPUT] = "`>'",
	[MS_LEX_OP_INPUT] = "`<'",
	[MS_LEX_OP_HEREDOC] = "`<<'",
	[MS_LEX_OP_PIPE] = "`|'",
};

const char *parser_strerror(t_lex_tok *tok)
{
	static char	full[100] = "syntax error near unexpected token ";
	char		*tok_str;

	tok_str = full + ft_strlen("syntax error near unexpected token ");
	if (tok->type == MS_LEX_TOK_OP)
		ft_strlcpy(tok_str, g_op_err_str[tok->op], 50);
	else if (tok->type == MS_LEX_TOK_ID)
	{
		ft_strlcpy(tok_str, "`", 50);
		if (ft_strlcat(tok_str, tok->id, 50) > 12)
			ft_strlcat(tok_str, "...", 50);
		ft_strlcat(tok_str, "'", 50);
	}
	else if (tok->type == MS_LEX_TOK_WS)
		ft_strlcpy(tok_str, "`whitespace'", 50);
	else if (tok->type == MS_LEX_TOK_VAR)
	{
		ft_strlcpy(tok_str, "`$", 50);
		if (ft_strlcat(tok_str, tok->id, 50) > 13)
			ft_strlcat(tok_str, "...", 50);
		ft_strlcat(tok_str, "'", 50);
	}
	return (full);
}
