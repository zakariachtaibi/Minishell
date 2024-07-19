/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:36:42 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/19 13:24:50 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_unclosed_double_quotes(char *temp)
{
	char	*input;
	int		i;
	int		c;

	input = temp;
	i = 0;
	c = 0;
	while (input[i])
	{
		if (input[i] == '"')
			c++;
		i++;
	}
	if (c % 2 == 0)
		return (0);
	else
		return (1);
}

int	has_mismatched_parentheses(char *temp)
{
	char	*input;
	int		i;
	int		c;

	input = temp;
	i = 0;
	c = 0;
	while (input[i])
	{
		if (input[i] == '(')
			c++;
		else if (input[i] == ')')
			c--;
		i++;
	}
	return (c);
}
