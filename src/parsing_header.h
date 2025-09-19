/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_header.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:59:36 by amel-fou          #+#    #+#             */
/*   Updated: 2025/09/19 11:31:37 by amel-fou         ###   ########.fr       */
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
		t_parse_tree	*root;
		int				quote;
		int				error; //might be necessary Idk
		int				escaping;
		char			*curr;
		int				curr_pos;
		int				start;
	}	t_parsing_context;

	void			add_node(t_parsing_context *par_con, char *string);
	size_t			ft_strlcpy(char *dst, const char *src, size_t size);
	void			ft_bzero(void *s, size_t n);
	int				second_pass(t_parsing_context *par_con);
	int				node_trim_splitter(t_parsing_context *par_con, t_token_list *node);
	void			init_parcon(t_parsing_context *par_con);
	char			*ft_substr(char const *s, unsigned int start, size_t len);
	size_t			ft_strlen(const char *s);
	char			*ft_strdup(const char *s);
	int				ismetachar(char c);
	int				metastate(t_parsing_context *par_con);
	char			*ft_substr_wrapper(t_parsing_context *par_con);
	t_token_list	*mk_node(char *string);
	void			meta_consumption(t_parsing_context *par_con, char *buf);
	int				double_meta_consumption(t_parsing_context *par_con, char *buf);
	void			quote_state_switch(t_parsing_context *par_con);
	void 			print_tokens(t_parsing_context *par_con);
	void			print_ast(t_parse_tree *node, int depth);
	void			token_assignation(t_token_list *node);
	int				op_finder(char *string);
	char			*example_trim(char *string);
	int				is_cmd(char *string);
	int				full_meta_check(char *string);
	int				node_quote_check(char *string);
	char			*ft_variable_exp_strdup(char *string);
	void			third_pass(t_parsing_context *par_con);
	t_parse_tree	*assemble_command_node(t_token_list *start, size_t count);
	void			attach_to_tree(t_parsing_context *par_con, t_parse_tree *node);
	t_parse_tree	*new_tree(t_ms_token tok);
	void			new_child(t_parse_tree *tree, t_parse_tree *sapling);
	void			third_pass(t_parsing_context *par_con);
	void			token_assignation(t_token_list *node);
	void			init_parcon(t_parsing_context *par_con);
	int				ft_strncmp(const char *s1, const char *s2, size_t size);
	int				ft_isalnum(int c);
	int				ft_isspace(char c);
	int				ismetachar(char c);
	int				first_pass(char *input, t_parsing_context *par_con);
	void			trim_space_nodes(t_parsing_context *par_con);
	void			re_organize_node(t_parsing_context *par_con,\
						t_token_list *cur, t_token_list *prev, t_token_list *to_free);
	void			space_node_free(t_token_list *to_free);
	void			exit_func(t_parsing_context *par_con);
	void			free_parse_tree(t_parse_tree *node);
	void			free_token_list(t_token_list *head);

		///////////////////// testers
	void			test_first_pass(t_parsing_context *par_con);
	void			test_second_pass(t_parsing_context *par_con);
	void			test_third_pass(t_parsing_context *par_con);
	void			print_ast(t_parse_tree *node, int depth);
	
	#endif