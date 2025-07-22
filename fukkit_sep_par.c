/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fukkit_sep_par.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/07/22 13:47:36 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_header.h"

int	ismetachar(char c)
{
	int	i;

	while(METACHARS[i])
	{
		if (METACHARS[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	single_qoute_lex(char *input, t_token_list **head, char *curr)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while(input[i] != '\0')
	{
		while(input[i] != '\'')
		{
			curr[j] = input[i]; //see if curr actually gets changed, you might just be sending temporal reference over.
			i++;
			j++;
		}
	}
	if (input[i] == '\0')
	{
		ft_printf("Error: no closing '\n");
		node_free(head); //freeing function for token list that bzeros head/removes all nodes for fresh start next iteration.
		return (-1); //make enum for error handling of incorrect usage
	}
}

int	double_qoute_lex(char *input, t_token_list **head, char *curr)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while(input[i] != '\0')
	{
		while(input[i] != '"')
		{
			curr[j] = input[i]; //see if curr actually gets changed, you might just be sending temporal reference over.
			j++;
			i++;
		}
	}
	if (input[i] == '\0')
	{
		ft_printf("Error: no closing \"\n");
		node_free(head); //freeing function for token list that bzeros head/removes all nodes for fresh start next iteration.
		return (-1); //make enum for error handling of incorrect usage
	}
}

parse_meta_char(char *input, t_token_list **head)
{
	int	i;
	int	j;
	char *curr;

	j = 0;
	i = 0;
	while(input[i] != '\0' && (ismetachar(input[i]) || isspace(input[i])))
	{
		while(isspace(input[i]))
		{
			i++;
		}
		if (ismetachar(input[i]))
		{
			curr[j++] = input[i];
			if (ismetachar(input[i + 1]))
			{
				curr[j++] = input[i + 1];
				add_node(curr, head); //ok this won't work bc it needs a string, make a curr.
				// also, make it assign it a metachar flag as well
				i++;
			}
			i++; //iteration is gonna be weird here, fix it
		}
	}
	return (i);
}

int	JUST_PARSE_IT_BRO(char *input, t_token_list **head)
{
	int	i;
	char *curr;
	int	temp;

	i = 0;
	while(input[i] != '\0')
	{
		if(ismetacharacter(input[i]))
		{
			temp += parse_meta_char(input[i], head);
			if (temp < 0) //invalid character spotted
			{
				free_head(head);
				exit (1); // for instance
			}
		}
	}
}

