/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:04:58 by amel-fou          #+#    #+#             */
/*   Updated: 2025/07/23 14:21:19 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parsing_header.h"
#include "tokens.h"
int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t')
		return (1);
	return (0);
}

int	ft_isredirapp(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '>' && str[i + 1] == '>')
		return (MS_FILE_APPEND);
	else
		return (-1);
}

int	ft_isheredoc(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '<' && str[i + 1] == '<')
		return (MS_HEREDOC);
	else
		return (-1);
}

int	ft_isoutput(char *str)
{
	if (str[0] = '>')
		return (MS_FILE_OUTPUT);
	else
		return (-1);
}

int	ft_isoutput(char *str)
{
	if (str[0] = '<')
		return (MS_FILE_INPUT);
	else
		return (-1);
}

int	ft_ispipe(char *str)
{
	if (str[0] = '|')
		return (MS_PIPE);
	else
		return (-1);
}