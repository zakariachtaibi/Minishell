/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:36:42 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/21 12:25:59 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	count_quotes_and_parentheses(char c, int *quote, int *dquote,
									int *parentheses)
{
	if (c == '\'' && *dquote % 2 == 0)
		(*quote)++;
	else if (c == '"' && *quote % 2 == 0)
		(*dquote)++;
	else if (c == '(')
		(*parentheses)++;
	else if (c == ')')
		(*parentheses)--;
}

int	has_unclosed_quotes_or_parentheses(char *temp)
{
	int	i;
	int	quote;
	int	dquote;
	int	parentheses;

	i = 0;
	quote = 0;
	dquote = 0;
	parentheses = 0;
	while (temp[i])
	{
		count_quotes_and_parentheses(temp[i], &quote, &dquote, &parentheses);
		i++;
	}
	if ((quote % 2 != 0) || (dquote % 2 != 0) || (parentheses != 0))
	{
		printf("Error: Unclosed quotes or parentheses\n");
		return (1);
	}
	return (0);
}

void	init_vars(int *i, int *j, int *array)
{
	*i = 0;
	*j = 0;
	*array = 0;
}

char	*remove_enclosing_chars(char *input)
{
	int		i;
	int		j;
	int		array[3];
	char	*new_input;

	init_vars(&i, &j, array);
	new_input = malloc(ft_strlen(input) + 1);
	if (!new_input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' && !array[1] && !array[2])
			array[0] = !array[0];
		else if (input[i] == '"' && !array[0] && !array[2])
			array[1] = !array[1];
		else if (input[i] == '(' && !array[0] && !array[1])
			array[2] = 1;
		else if (input[i] == ')' && !array[0] && !array[1] && array[2])
			array[2] = 0;
		else
			new_input[j++] = input[i];
		i++;
	}
	new_input[j] = '\0';
	return (new_input);
}
