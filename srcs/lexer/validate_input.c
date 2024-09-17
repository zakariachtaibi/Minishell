/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:09:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/09/16 17:45:52 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*validat_input(char *input, t_tools *tools)
{
	if (has_unclosed_quotes_or_parentheses(input, tools))
		return(NULL);
	 if(has_semicolon(input, tools))
		return (NULL);
	return(input);
}


