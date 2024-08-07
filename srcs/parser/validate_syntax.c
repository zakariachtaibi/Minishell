/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:36:11 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/04 13:23:09 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lexical	*validate_syntax(t_lexical *tokens)
{
	if (has_invalid_redirections(tokens))
	{
		if (has_invalid_redirections(tokens) == 10)
			printf(" syntax error near unexpected token '%s'\n", (tokens)->str);
		else if ((tokens)->next == NULL)
			printf(" syntax error near unexpected token 'newline' \n");
		else
			printf(" syntax error near unexpected token '%s'\n", (tokens)->next->str);
		return (NULL);
	}
	return (tokens);
}
