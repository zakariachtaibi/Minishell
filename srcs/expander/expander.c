/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/27 21:02:10 by mac              ###   ########.fr       */
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

char *get_vars(const char *current_word, size_t *j, t_tools *tools)
{
	size_t	var_start;
	char	*var_name;
	char	*var_value;

	var_value = NULL;
	var_start = *j;
	while (current_word[*j] && (ft_isalnum(current_word[*j])
		|| current_word[*j] == '_'))
			(*j)++;
		var_name = ft_strndup(&current_word[var_start], *j - var_start);
		var_value = get_vars_value(var_name, tools);
		free(var_name);
		if (!var_value)
			var_value = ft_strdup("");
	return(var_value);
}

char *expand_plain2(const char *current_word, char *expanded_word,  size_t *j)
{
	char	temp_str[2];
	char	*new_expanded_word;
	
	temp_str[0] = '\0';
	temp_str[0] = current_word[*j];
	temp_str[1] = '\0';
	(*j)++;
	new_expanded_word = ft_strjoin(expanded_word, temp_str);
	free(expanded_word);
	expanded_word = new_expanded_word;
	return(expanded_word);
}

char *expand_if_dollar(const char *current_word, char *expanded_word,  size_t *j, t_tools *tools)
{
	char	*new_expanded_word;
	char	*var_value;

	var_value = NULL;
	(*j)++;
	if (ft_isdigit(current_word[*j]) || current_word[*j] == '$' || current_word[*j] == '?')
	{
		if (current_word[*j] == '$')
			var_value = ft_itoa(getpid());
		else if (current_word[*j] == '?')
			var_value = ft_itoa(tools->exit_status);
			(*j)++;
	}
	else if (current_word[*j] == '\'')
		return (expanded_word);
	else if (ft_isalnum(current_word[*j]) || current_word[*j] == '_')
		var_value = get_vars(current_word, j, tools);
	else
		var_value = ft_strdup("$");
	new_expanded_word = ft_strjoin(expanded_word, var_value);
	free(expanded_word);
	expanded_word = new_expanded_word;
	return (expanded_word);
}

char	*expand_double_quote(t_tools *tools, const char *current_word,
		size_t *j, int heredoc_flag)
{
	char	*expanded_word;

	expanded_word = ft_strdup("");
	while (current_word[*j] && current_word[*j] != '"')
	{
		if (current_word[*j] == '$' && heredoc_flag == 0)
			expanded_word = expand_if_dollar(current_word, expanded_word, j, tools);
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

char	*expand_vars(t_tools *tools, char *current_word, int *flag,
		int heredoc_flag)
{
	size_t	j;
	char	*expanded_word;
	size_t	len;
	char	*new_expansion;
	char	*temp_word;

	j = 0;
	expanded_word = NULL;
	len = ft_strlen(current_word);
	while (j < len)
	{
		new_expansion = NULL;
		if (current_word[j] == '\'')
		{
			new_expansion = expand_single_quote(current_word, &j);
			*flag = 0;
		}
		else if (current_word[j] == '"')
		{
			j++;
			new_expansion = expand_double_quote(tools, current_word, &j,
					heredoc_flag);
			*flag = 0;
		}
		else if (current_word[j] == '$' && heredoc_flag == 0)
		{
			if (j + 1 < len && ft_isdigit(current_word[j + 1]))
				j += 2;
			else if (j + 1 < len && current_word[j + 1] == '$')
			{
				new_expansion = ft_itoa(getpid());
				j += 2;
				*flag = 1;
			}
			else if (j + 1 < len && (current_word[j + 1] == '?'
						|| ft_isalnum(current_word[j + 1]) || current_word[j
						+ 1] == '_'))
			{
				new_expansion = expand_variable(tools, current_word, &j);
				*flag = 1;
			}
			else
			{
				if (current_word[j + 1] == '\0')
					new_expansion = ft_strdup("$");
				else
					new_expansion = ft_strdup("");
				j++;
				*flag = 0;
			}
		}
		else
		{
			new_expansion = expand_plain_text(current_word, &j);
			*flag = 2;
		}
		if (new_expansion)
		{
			temp_word = ft_strjoin(expanded_word, new_expansion);
			free(expanded_word);
			free(new_expansion);
			expanded_word = temp_word;
		}
	}
	return (expanded_word);
}
