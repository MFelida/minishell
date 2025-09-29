/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mifelida <mifelida@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:49:46 by mifelida          #+#    #+#             */
/*   Updated: 2025/09/26 16:50:01 by mifelida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "parse_tree.h"

t_parse_node	*get_parse_tree(char *input);
void			free_parse_tree(t_parse_node **root);

#endif
