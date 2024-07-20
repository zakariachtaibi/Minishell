/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:36:11 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 21:17:47 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lexical *validate_syntax(t_lexical *tokens)
{
    if (has_invalid_redirections(tokens))
    {
        if(has_invalid_redirections(tokens) == 10)
             printf(" syntax error near unexpected token '%s'\n", (tokens)->str);
        else if ((tokens)->next == NULL)
            printf(" syntax error near unexpected token 'newline' \n");
        else
            printf(" syntax error near unexpected token '%s'\n", (tokens)->next->str);
        return (NULL);
    }
   return (tokens);
}