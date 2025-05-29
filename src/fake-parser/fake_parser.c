/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:05:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/29 21:57:45 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fake_parser.h"
#include "libft.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

t_parse_node	*get_next_node(int fd);
void			fp_print(t_parse_node node, int tab_depth);

#define FP_WHITESPACE	"\t\v\f "

char	*skip_whitespace(char *str)
{
	while (ft_strchr(FP_WHITESPACE, *str))
		str++;
	return (str);
}

int	count_chld_nodes(t_parse_node *parent)
{
	int				res;
	t_parse_node	**chld_arr;

	res = 0;
	chld_arr = parent->children;
	while (*(chld_arr++) != NULL)
		res++;
	return (res);
}

t_parse_node	*get_id_node(char *str)
{
	t_parse_node	*res;

	res = malloc(sizeof(t_parse_node));
	res->tok.type = FP_TOK_IDENTIFIER;
	str[ft_strlen(str) - 1] = '\0';
	str = ft_strchr(str, '-') + 2;
	res->tok.id.value = ft_strdup(str);
	return (res);
}

t_parse_node	*get_cmd_node(int fd)
{
	t_parse_node	*res;
	char			*line;
	int				i;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.type = FP_TOK_OP, .op.op = FP_OP_CMD};
	res->children = ft_calloc(50, sizeof(t_parse_node*));
	i = 0;
	while ((line = ft_gnl(fd)) && ft_strncmp(skip_whitespace(line), "\n", 1))
	{
		res->children[i] = get_id_node(line);
		free(line);
		i++;
	}
	res->children[i] = NULL;
	return (res);
}

t_parse_node	*get_pipe_node(int fd)
{
	t_parse_node	*res;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.op = FP_OP_PIPE, .op.type = FP_TOK_OP};
	res->children = ft_calloc(3, sizeof(t_parse_node*));
	res->children[0] = get_next_node(fd);
	res->children[1] = get_next_node(fd);
	return (res);
}

t_parse_node	*get_next_node(int fd)
{
	char	*line;

	line = ft_gnl(fd);
	if (!ft_strncmp(skip_whitespace(line), "cmd", 3))
		return (free(line), get_cmd_node(fd));
	if (!ft_strncmp(skip_whitespace(line), "pipe", 4))
		return (free(line), get_pipe_node(fd));
	return (NULL);
}

t_parse_node	*get_parse_tree(char *file)
{
	int				fd;
	char			*err_msg;
	t_parse_node	*res;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		err_msg = strerror(errno);
		ft_fprintf(2, "%s: %s: %s", __FILE_NAME__, file, err_msg);
		exit(EXIT_FAILURE);
	}
	res = get_next_node(fd);
	close(fd);
	fp_print(*res, 0);
	return (res);
}
