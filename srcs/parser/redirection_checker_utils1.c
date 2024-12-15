/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker_utils1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 22:32:34 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/12/14 22:39:42 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_dollar_special(char *input, size_t *i, size_t len)
{
	if (*i + 1 < len && ft_isdigit(input[*i + 1]))
	{
		*i += 2;
		return (NULL);
	}
	if (*i + 1 < len && input[*i + 1] == '$')
	{
		*i += 2;
		return (ft_itoa(getpid()));
	}
	return (NULL);
}

char	*expand_var(char *input, size_t *i, t_tools *tools, size_t len)
{
	char	*new_expansion;

	new_expansion = handle_dollar_special(input, i, len);
	if (new_expansion)
		return (new_expansion);
	if (*i + 1 < len && (input[*i + 1] == '?' || ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_'))
		return (expand_variable(tools, input, i));
	if (input[*i + 1] == '\0')
		new_expansion = ft_strdup("$");
	else
		new_expansion = ft_strdup("");
	*i += 1;
	return (new_expansion);
}
