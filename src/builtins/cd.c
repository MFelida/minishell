/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:35:37 by mifelida          #+#    #+#             */
/*   Updated: 2025/08/29 14:36:34 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "exit_statuses.h"
#include "utils.h"

int	ms_cd(char **args, t_cmd_params *params, ...)
{
	if (!args)
		return (MS_CMD_ERROR_OK);
	params->wstatus = _set_wstatus(MS_FAILURE, 0);
	return (MS_CMD_ERROR_OK);
}
