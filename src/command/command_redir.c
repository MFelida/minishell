/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:43:22 by mifelida          #+#    #+#             */
/*   Updated: 2025/10/15 13:57:25 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "libft.h"
#include "redirect.h"

#include <fcntl.h>
#include <stdlib.h>

int	cmd_hear_doc(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
			(t_redir_dest){.type = MS_REDIR_FD, .fd = node->tok.op.value},
			(t_redir_src){.type = MS_REDIR_FD, .fd = STDIN_FILENO});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		ft_lstclear((t_list **) &last->next, free);
	return (retval);
}

int	cmd_input_redir(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
			(t_redir_dest){.type = MS_REDIR_FILE,
			.file = node->children[0]->tok.id.value,
			.mode = O_RDONLY},
			(t_redir_src){.type = MS_REDIR_FD, .fd = STDIN_FILENO});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}

int	cmd_output_redir(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
			(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
			(t_redir_dest){.type = MS_REDIR_FILE,
			.file = node->children[0]->tok.id.value,
			.mode = O_WRONLY | O_CREAT | O_TRUNC, .flags = 0644});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}

int	cmd_output_append(t_cmd_params params, t_parse_node *node)
{
	int		retval;
	t_list	*last;

	last = ft_lstlast((t_list *) params.redirs);
	retval = add_redir(&params,
			(t_redir_src){.type = MS_REDIR_FD, .fd = STDOUT_FILENO},
			(t_redir_dest){.type = MS_REDIR_FILE,
			.file = node->children[0]->tok.id.value,
			.mode = O_APPEND | O_CREAT | O_WRONLY, .flags = 0644});
	if (retval)
		return (retval);
	retval = cmd_next_node(&params, node->children[1]);
	free(ft_lstlast((t_list *) params.redirs));
	if (last)
		last->next = NULL;
	return (retval);
}
