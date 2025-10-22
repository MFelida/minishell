/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:33:11 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/30 13:40:46 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "parse_tree.h"

int		exec_parsetree(t_parse_node	**pt);
void	setup_sighandlers(void);
void	reset_signal(void);
void	forward_sigint(t_cmd_params *params);
int		ms_is_interactive(void);
