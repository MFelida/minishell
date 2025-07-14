/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fukkit_sep_par.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amel-fou <amel-fou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:30:24 by amel-fou          #+#    #+#             */
/*   Updated: 2025/07/14 12:16:09 by amel-fou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	JUST_PARSE_IT_BRO(char *input, t_token_type **head)
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
				
			}
		}
	}
}

parse_meta_char(input[i], head)
{
	
}