/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fake_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:05:55 by mifelida          #+#    #+#             */
/*   Updated: 2025/05/30 15:30:06 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "fake_parser.h"
#include "file_format.h"
#include "libft.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

t_parse_node	*get_next_node(int fd);
void			fp_print(t_parse_node node, int tab_depth);

#define FP_WHITESPACE	"\t\v\f "

char	*skip_ws(char *str)
{
	while (ft_strchr(FP_WHITESPACE, *str))
		str++;
	return (str);
}

void	skip_line(int fd)
{
	free(ft_gnl(fd));
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
	if (*str == '$')
		res->tok.id.value = ft_strdup(ms_getenv(str + 1));
	else
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
	res->children = ft_calloc(50, sizeof (t_parse_node *));
	i = 0;
	while ((line = ft_gnl(fd)) && ft_strncmp(skip_ws(line), "\n", 1))
	{
		if (!ft_strncmp(skip_ws(line), "\n", 1))
		{
			free(line);
			break;
		}
		res->children[i] = get_id_node(line);
		free(line);
		i++;
	}
	free(line);
	res->children[i] = NULL;
	return (res);
}

t_parse_node	*get_bltin_node(int fd)
{
	t_parse_node	*res;
	char			*line;
	int				i;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.type = FP_TOK_OP, .op.op = FP_OP_BLTIN};
	res->children = ft_calloc(50, sizeof (t_parse_node *));
	i = 0;
	while ((line = ft_gnl(fd)) && ft_strncmp(skip_ws(line), "\n", 1))
	{
		if (!ft_strncmp(skip_ws(line), "\n", 1))
		{
			free(line);
			break;
		}
		res->children[i] = get_id_node(line);
		free(line);
		i++;
	}
	free(line);
	res->children[i] = NULL;
	return (res);
}

t_parse_node	*get_pipe_node(int fd)
{
	t_parse_node	*res;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.op = FP_OP_PIPE, .op.type = FP_TOK_OP};
	res->children = ft_calloc(3, sizeof(t_parse_node *));
	res->children[0] = get_next_node(fd);
	res->children[1] = get_next_node(fd);
	return (res);
}

t_parse_node	*get_input_node(int fd)
{
	t_parse_node	*res;
	char			*line;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.op = FP_OP_FILE_INPUT, .op.type = FP_TOK_OP};
	res->children = ft_calloc(3, sizeof(t_parse_node *));
	line = ft_gnl(fd);
	res->children[0] = get_id_node(line);
	free(line);
	skip_line(fd);
	res->children[1] = get_next_node(fd);
	return (res);
}

t_parse_node	*get_output_node(int fd)
{
	t_parse_node	*res;
	char			*line;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.op = FP_OP_FILE_OUTPUT, .op.type = FP_TOK_OP};
	res->children = ft_calloc(3, sizeof(t_parse_node *));
	res->children[0] = get_next_node(fd);
	line = ft_gnl(fd);
	res->children[1] = get_id_node(line);
	free(line);
	return (res);
}

t_parse_node	*get_append_node(int fd)
{
	t_parse_node	*res;
	char			*line;

	res = malloc(sizeof(t_parse_node));
	res->tok = (t_fp_token){.op.op = FP_OP_FILE_APPEND, .op.type = FP_TOK_OP};
	res->children = ft_calloc(3, sizeof(t_parse_node *));
	res->children[0] = get_next_node(fd);
	line = ft_gnl(fd);
	res->children[1] = get_id_node(line);
	free(line);
	return (res);
}

t_parse_node	*get_next_node(int fd)
{
	char		*line;
	t_fp_ops	op;

	line = ft_gnl(fd);
	op = 0;
	while (op < sizeof(g_op_strings)
		&& ft_strncmp(skip_ws(line),
			g_op_strings[op], ft_strlen(g_op_strings[op])))
		op++;
	free(line);
	if (op == FP_OP_ERROR || op >= sizeof(g_op_strings))
		return (NULL);
	return (g_node_getters[op](fd));
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
		ft_exit(EXIT_FAILURE);
	}
	res = get_next_node(fd);
	close(fd);
	// fp_print(*res, 0);
	return (res);
}
