/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:34:14 by mifelida          #+#    #+#             */
/*   Updated: 2025/08/29 14:36:50 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"

int	ms_env(t_cmd_params params)
{
	if (params.cmd_args)
		return (MS_FAILURE);
	return (MS_SUCCESS);
}
