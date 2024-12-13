/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:07:54 by mac               #+#    #+#             */
/*   Updated: 2024/12/13 02:25:10 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_single_quote(const char *current_word, size_t *j, int *flag)
{
	*flag = 0;
	return (expand_single_quote(current_word, j));
}

char	*handle_double_quote(t_tools *tools, const char *current_word,
		size_t *j)
{
	tools->flag = 0;
	(*j)++;
	return (expand_double_quote(tools, current_word, j));
}

char	*handle_double_dollar(t_tools *tools, size_t *j)
{
	(*j) += 2;
	tools->flag = 1;
	return (ft_itoa(getpid()));
}

char	*handle_dollar_sign(t_tools *tools, const char *current_word, size_t *j,
		size_t len)
{
	if (tools->heredoc_flag == 1)
		return (NULL);
	if (*j + 1 < len && ft_isdigit(current_word[*j + 1]))
	{
		(*j) += 2;
		return (NULL);
	}
	else if (*j + 1 < len && current_word[*j + 1] == '$')
		return(handle_double_dollar(tools, j));
	else if (*j + 1 < len && (current_word[*j + 1] == '?'
			|| ft_isalnum(current_word[*j + 1]) || current_word[*j
				+ 1] == '_'))
	{
		tools->flag = 1;
		return (expand_variable(tools, current_word, j));
	}
	else
	{
		(*j)++;
		tools->flag = 0;
		if (*j == len)
			return (ft_strdup("$"));
		return (ft_strdup(""));
	}
}
