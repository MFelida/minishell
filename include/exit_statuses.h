/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_statuses.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:22:07 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/02 18:03:58 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_STATUSES_H
# define EXIT_STATUSES_H

enum e_retvals
{
	MS_SUCCES = 0,
	MS_FAILURE = 1,
	MS_BUILTIN_MISUSE = 2,
	MS_PERM_DENIED = 126,
	MS_CMD_NOT_FOUND = 127,
	MS_SIGNAL_EXIT = 128,
};
#endif // !EXIT_STATUSES_H
