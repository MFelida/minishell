/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:51:58 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/26 17:56:01 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "utils.h"

#include <stddef.h>
#include <stdlib.h>

// TODO: Remove unhandled metacharacters?
#define METACHARS " \t\n|&;,<>"

t_lex_lst	*new_lex_lst(char *str, size_t start, size_t len)
{
	t_lex_lst	*res;
	
	res = ft_calloc(1, sizeof(t_lex_lst));
	if (!res)
		return (NULL);
	res->str = ft_substr(str, start, len);
	if (!res->str)
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	is_meta(const char c)
{
	return (!!ft_strchr(METACHARS, c));
}

t_lex_lst	*handle_ws(char **str)
{
	char		*new_str;
	t_lex_lst	*res;

	*str = skip_ws(*str);
	new_str = ft_strdup(" ");
	if (!new_str)
		return (NULL);
	res = new_lex_lst(new_str, 0, ft_strlen(new_str));
	if (!res)
	{
		free(new_str);
		return (NULL);
	}
	return (res);
}

t_lex_lst	*handle_bar(char **str)
{
	char		*new_str;
	t_lex_lst	*res;

	*str += 1;
	new_str = ft_strdup("|");
	if (!new_str)
		return (NULL);
	res = new_lex_lst(new_str, 0, ft_strlen(new_str));
	if (!res)
	{
		free(new_str);
		return (NULL);
	}
	return (res);
}

t_lex_lst	*handle_less(char **str)
{
	char		*new_str;
	t_lex_lst	*res;

	*str += 1;
	if (**str == '<')
	{
		*str += 1;
		new_str = ft_strdup("<<");
	}
	else
		new_str = ft_strdup("<");
	if (!new_str)
		return (NULL);
	res = new_lex_lst(new_str, 0, ft_strlen(new_str));
	if (!res)
	{
		free(new_str);
		return (NULL);
	}
	return (res);
}

t_lex_lst	*handle_greater(char **str)
{
	char		*new_str;
	t_lex_lst	*res;

	*str += 1;
	if (**str == '>')
	{
		*str += 1;
		new_str = ft_strdup(">>");
	}
	else
		new_str = ft_strdup(">");
	if (!new_str)
		return (NULL);
	res = new_lex_lst(new_str, 0, ft_strlen(new_str));
	if (!res)
	{
		free(new_str);
		return (NULL);
	}
	return (res);
}

t_lex_lst	*handle_meta(char **str)
{
	if (is_ws(**str))
		return (handle_ws(str));
	if (**str == '|')
		return (handle_bar(str));
	if (**str == '<')
		return (handle_less(str));
	if (**str == '>')
		return (handle_greater(str));
	return (NULL);
}

t_lex_lst	*lex_input(char *input)
{
	t_lex_lst	*res;
	char		*walker;
	t_lex_lst	*new_node;

	while (input)
	{
		walker = input;
		if (is_meta(*input))
			new_node = handle_meta(&input);
		else
		{
			while (!is_meta(*walker))
				walker++;
			new_node = new_lex_lst(input, 0, walker - input);
			input = walker;
		}
		if (!new_node)
		{
			free_lex_list(res);
			return (NULL);
		}
		ft_lstadd_back((t_list **) &res, (t_list *) new_node);
	}
	return (res);
}
