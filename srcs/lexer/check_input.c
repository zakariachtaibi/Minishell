/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:09:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/19 16:28:08 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_unclosed_quotes(char *temp)
{
	int	i;
	int	quote;
	int	dquote;

	i = 0;
	quote = 0;
	dquote = 0;
	while (temp[i])
	{
		if (temp[i] == '\'' && dquote % 2 == 0)
			quote++;
		else if (temp[i] == '"' && quote % 2 == 0)
			dquote++;
		i++;
	}
	return (dquote % 2 == 0 && quote % 2 == 0);
}

void check_input(char *input)
{
    if (!has_unclosed_quotes(input))
	{
		printf("Error: Unclosed quotes\n");
        free(input);
        exit(1);
	}
}