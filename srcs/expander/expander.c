/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/12/15 22:30:20 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_single_quote(const char *current_word, size_t *j)
{
	char	temp_str[2];
	char	*expanded_word;
	char	*new_expanded_word;

	expanded_word = ft_strdup("");
	(*j)++;
	while (current_word[*j] && current_word[*j] != '\'')
	{
		temp_str[0] = current_word[*j];
		temp_str[1] = '\0';
		new_expanded_word = ft_strjoin(expanded_word, temp_str);
		free(expanded_word);
		expanded_word = new_expanded_word;
		(*j)++;
	}
	(*j)++;
	return (expanded_word);
}

char	*expand_double_quote(t_tools *tools, const char *current_word,
		size_t *j)
{
	char	*expanded_word;

	expanded_word = ft_strdup("");
	while (current_word[*j] && current_word[*j] != '"')
	{
		if (current_word[*j] == '$' && tools->heredoc_flag == 0)
			expanded_word = expand_if_dollar(current_word, expanded_word, j,
					tools);
		else
			expanded_word = expand_plain2(current_word, expanded_word, j);
	}
	(*j)++;
	return (expanded_word);
}

char	*expand_variable(t_tools *tools, const char *current_word, size_t *j)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;

	(*j)++;
	if (current_word[*j] == '?')
	{
		(*j)++;
		return (ft_itoa(tools->exit_status));
	}
	var_start = *j;
	while (current_word[*j] && (ft_isalnum(current_word[*j])
			|| current_word[*j] == '_'))
		(*j)++;
	var_len = *j - var_start;
	var_name = ft_strndup(&current_word[var_start], var_len);
	var_value = get_vars_value(var_name, tools);
	free(var_name);
	if (!var_value)
		return (NULL);
	return (ft_strdup(var_value));
}

char	*expand_plain_text(const char *current_word, size_t *j)
{
	char	temp_str[2];

	temp_str[0] = current_word[*j];
	temp_str[1] = '\0';
	(*j)++;
	return (ft_strdup(temp_str));
}
