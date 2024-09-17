/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:36:11 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/17 11:48:41 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_invalid_redirections(t_lexical *tokens)
{
	while ((tokens))
	{
		
		if ((tokens)->token != 5 && (tokens)->token != 0 )
		{
			// if ((tokens)->token == 0)
			// {
			// 	if ((tokens)->prev == NULL)
			// 		return (10);
			// }
			if (((tokens)->next == NULL ) || (((tokens)->next)->token != 5))
				return (1);
		}
		(tokens) = (tokens)->next;
	}
	return (0);
}

t_lexical	*validate_syntax(t_lexical *tokens, t_tools *tools)
{
	if (has_invalid_redirections(tokens))
	{
		if (has_invalid_redirections(tokens) == 10)
		{
			printf(" syntax error near unexpected token '%s'\n", (tokens)->str);
			tools->exit_status = 2;
		}
		else if ((tokens)->next == NULL)
		{
			printf(" syntax error near unexpected token 'newline' \n");
			tools->exit_status = 2;
		}
		else
		{
			printf(" syntax error near unexpected token '%s'\n", (tokens)->next->str);
			tools->exit_status = 2;
		}
		return (NULL);
	}
	return (tokens);
}
