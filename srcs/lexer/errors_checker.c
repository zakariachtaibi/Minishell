/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:36:42 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/21 13:29:40 by hchouai          ###   ########.fr       */
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

int	has_unclosed_quotes_or_parentheses(char *temp, t_tools *tools)
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
		tools->exit_status = 2;
		return (1);
	}
	return (0);
}

void	init_vars(int *single_quote_open, int *double_quote_open,
		int *parentheses_open)
{
	*single_quote_open = 0;
	*double_quote_open = 0;
	*parentheses_open = 0;
}

int	has_semicolon(char *input, t_tools *tools)
{
	if ((strchr(input, ';')) || (strchr(input, '\\')))
	{
		printf("Error: invalid input\n");
		tools->exit_status = 2;
		return (1);
	}
	return (0);
}
