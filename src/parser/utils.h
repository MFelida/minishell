/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 19:43:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/28 22:37:45 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "parse_tree.h"
# include "lexer.h"

typedef	struct s_parse_context
{
	t_parse_node	*root;
	t_parse_node	**current_cmd_node;
}	t_parse_context;

const char		*parser_strerror(t_lex_tok *tok);

t_parse_node	*get_input_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_heredoc_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_output_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_append_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_pipe_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_op_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_id_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_ws_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_var_node(t_lex_tok **lex_list, t_parse_context *context);
t_parse_node	*get_cmd_node(t_lex_tok **lex_list, t_parse_context *context);

#endif
