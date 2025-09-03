/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:59:36 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/01 16:14:00 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HEADER_H
# define PARSING_HEADER_H
#include "tokens.h"
#include <stdlib.h>
#include <stddef.h>

#define METACHARS "<|>?*[]${}()&;= "

typedef struct s_token_list
{
	struct s_token_list *next;
	char *string;
	t_ms_token *type;
}	t_token_list;

typedef enum argument_state
{
NOT_IN_QUOTES,
ESCAPING,
IN_SINGLE_QUOTE,
IN_DOUBLE_QUOTE,
} t_argument_state;

typedef enum quote_state
{
	NOT_ESCAPING = 0,
	SINGLE_ESCAPING,
}	t_quote_state;

typedef enum double_quote_state
{
	DOUBLE_ESCAPING,
}	t_double_quote_state;

int	ft_isspace(char c);

typedef struct s_parsing_context
{

	char			*arg;
	int				pos;
	t_token_list	*head;
	t_token_list	*tail;
	int				quote;
	int				error; //might be necessary Idk
	int				escaping;
	char			*curr;
	int				curr_pos;
	int				start;
}	t_parsing_context;

void			add_node(t_parsing_context *par_con, char *string);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
int				single_qoute_lex(char *input, t_token_list **head, char *curr);
int				double_qoute_lex(char *input, t_token_list **head, char *curr);
void			ft_bzero(void *s, size_t n);
int				second_pass(t_parsing_context *par_con);
//void			quote_assign(t_token_list *head);
int				node_trim_splitter(t_parsing_context *par_con, t_token_list *node);
void			init_parcon(t_parsing_context *par_con);
char			*ft_substr(char const *s, unsigned int start, size_t len);
size_t			ft_strlen(const char *s);
char			*ft_strdup(const char *s);
int				ismetachar(char c);
int				metastate(t_parsing_context *par_con);
char			*ft_substr_wrapper(t_parsing_context *par_con);
t_token_list	*mk_node(char *string);
//void	add_node(t_parsing_context *par_con,char *string, t_token_list *head);

#endif