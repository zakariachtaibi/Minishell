/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:55:58 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/12 19:44:26 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_env_vars(t_tools *tools, char **envp)
{
	t_env_var	*new_var;

	tools->env_vars = NULL;
	while (*envp)
	{
		new_var = create_env_var(*envp);
		if (new_var)
			append_env_var(&tools->env_vars, new_var);
		envp++;
	}
}

char	*get_vars_value(char *str, t_tools *tools)
{
	t_env_var	*current;

	current = tools->env_vars;
	if (ft_strchr(str, '~'))
	{
		while (current)
		{
			if (!(ft_strcmp("HOME", current->key)))
				return (current->value);
			current = current->next;
		}
	}
	current = tools->env_vars;
	while (current)
	{
		if (!(ft_strcmp(str, current->key)))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*get_vars(const char *current_word, size_t *j, t_tools *tools)
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
	return (var_value);
}

char	*expand_if_dollar(const char *current_word, char *expanded_word,
	size_t *j, t_tools *tools)
{
	char	*new_expanded_word;
	char	*var_value;

	var_value = NULL;
	(*j)++;
	if (ft_isdigit(current_word[*j]) || current_word[*j] == '$'
		|| current_word[*j] == '?')
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

char	*expand_plain2(const char *current_word, char *expanded_word, size_t *j)
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
	return (expanded_word);
}
