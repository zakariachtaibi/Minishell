/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:09:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/08/30 09:37:22 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*validat_input(char *input)
{
	if (has_unclosed_quotes_or_parentheses(input))
		return(NULL);
	 if(has_semicolon(input))
		return (NULL);
	return(input);
}


