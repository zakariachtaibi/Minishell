/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:09:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/21 12:04:09 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*validat_input(char *input)
{
	char	*new_input;

	if (has_unclosed_quotes_or_parentheses(input))
	{
		free(input);
		return (NULL);
	}
	new_input = remove_enclosing_chars(input);
	if (!new_input)
	{
		free(input);
		return (NULL);
	}
	return (new_input);
}
