/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:36:11 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/10 18:33:43 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_invalid_redirections(t_lexical **tokens)
{
	t_lexical	*current;

	current = *tokens;
	while ((current))
	{
		if (current->token != 5 && current->token != 0)
		{
			if ((current->next == NULL) || ((current->next)->token != 5))
				return (1);
		}
		else if (current->token == 0)
		{
			if ((current->next == NULL) || ((current->next)->token == 0)
				|| (current->prev == NULL))
				return (10);
		}
		current = current->next;
	}
	return (0);
}

t_lexical	*validate_syntax(t_lexical *tokens, t_tools *tools)
{
	t_lexical	*temp;
	int			error_code;

	temp = tokens;
	error_code = has_invalid_redirections(&temp);
	if (error_code)
	{
		if (error_code == 10)
			printf("syntax error near unexpected token '%s'\n", tokens->str);
		else if (!tokens->next)
			printf("syntax error near unexpected token 'newline'\n");
		else
			printf("syntax error near unexpected token '%s'\n",
				tokens->next->str);
		tools->exit_status = 2;
		free_lexical(tokens);
		return (NULL);
	}
	return (tokens);
}
