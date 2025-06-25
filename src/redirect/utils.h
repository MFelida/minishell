/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:48:14 by mifelida          #+#    #+#             */
/*   Updated: 2025/06/25 14:49:39 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "redirect_types.h"

t_redir			*_new_redir(void);
t_redir_error	_get_src_dest(t_redir_src *d, t_redir_src *r);

#endif // !UTILS_H
