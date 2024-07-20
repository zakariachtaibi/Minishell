/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:15:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 21:17:26 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_invalid_redirections(t_lexical *tokens)
{
	
	while ((tokens))
	{
		if ((tokens)->token != 5)
		{
			if((tokens)->token == 0)
				{
					if((tokens)->prev == NULL)
						return(10);
				}
			if (((tokens)->next == NULL ) || (((tokens)->next)->token != 5))
				return (1);
		}
		(tokens) = (tokens)->next;
	}
	return (0);
}
