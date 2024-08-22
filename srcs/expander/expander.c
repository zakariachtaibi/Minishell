/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/08/21 11:55:06 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_env_vars(t_tools *tools, char **envp)
{
	tools->env_vars = NULL;
	t_env_var *head = NULL;

	while (*envp)
	{
		char *equal_sign = ft_strchr(*envp, '=');
		if (equal_sign)
		{
			t_env_var *new_var = malloc(sizeof(t_env_var));
			new_var->key = ft_substr(*envp, 0, equal_sign - *envp);
			new_var->value = ft_strdup(equal_sign + 1);
			new_var->next = NULL;
			if (tools->env_vars == NULL)
			{
				tools->env_vars = new_var;
				head = new_var;
			}
			else
			{
				head->next = new_var;
				head = new_var;
			}
		}
		envp++;
	}
}

char	*get_vars_value(char *str, t_tools *tools)
{
	// char		*value;
	t_env_var	*current;
	
	current = tools->env_vars;
	while(current)
	{
		if(!(strcmp(str,current->key)))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char *expand_single_quote(const char *current_word, size_t *j)
{
    char temp_str[2];
    char *expanded_word = ft_strdup("");
    (*j)++;
    while (current_word[*j] && current_word[*j] != '\'')
    {
        temp_str[0] = current_word[*j];
        temp_str[1] = '\0';
        char *new_expanded_word = ft_strjoin(expanded_word, temp_str);
        free(expanded_word);
        expanded_word = new_expanded_word;
        (*j)++;
    }
    (*j)++;
    return expanded_word;
}

char *expand_double_quote(t_tools *tools, const char *current_word, size_t *j)
{
    char *expanded_word = ft_strdup("");
    while (current_word[*j] && current_word[*j] != '"')
    {
        if (current_word[*j] == '$')
        {
            (*j)++;
            size_t var_start = *j;
            while (current_word[*j] && (ft_isalnum(current_word[*j]) || current_word[*j] == '_'))
                (*j)++;
            size_t var_len = *j - var_start;
            char *var_name = ft_strndup(&current_word[var_start], var_len);
            char *var_value = get_vars_value(var_name, tools);
            free(var_name);
            char *new_expanded_word = ft_strjoin(expanded_word, var_value);
            free(expanded_word);
            expanded_word = new_expanded_word;
        }
        else
        {
            char temp_str[2] = {current_word[*j], '\0'};
            char *new_expanded_word = ft_strjoin(expanded_word, temp_str);
            free(expanded_word);
            expanded_word = new_expanded_word;
            (*j)++;
        }
    }
    (*j)++;
    return expanded_word;
}

char *expand_variable(t_tools *tools, const char *current_word, size_t *j)
{
    (*j)++;
    size_t var_start = *j;
    while (current_word[*j] && (ft_isalnum(current_word[*j]) || current_word[*j] == '_'))
        (*j)++;
    size_t var_len = *j - var_start;
    char *var_name = ft_strndup(&current_word[var_start], var_len);
    char *var_value = get_vars_value(var_name, tools);
    free(var_name);
    if(!var_value)
        return(NULL);
    return ft_strdup(var_value);
}

char *expand_plain_text(const char *current_word, size_t *j)
{
    char temp_str[2] = {current_word[*j], '\0'};
    (*j)++;
    return ft_strdup(temp_str);
}

char *expand_vars(t_tools *tools, t_lexical *temp)
{
    size_t j = 0;
    char *expanded_word = ft_strdup("");
    char *current_word = temp->str;
    size_t len = ft_strlen(current_word);

    while (j < len)
    {
        char *new_expansion = NULL;
        if (current_word[j] == '\'')
            new_expansion = expand_single_quote(current_word, &j);
        else if (current_word[j] == '"')
        {
            j++;
            new_expansion = expand_double_quote(tools, current_word, &j);
        }
        else if (current_word[j] == '$')
            new_expansion = expand_variable(tools, current_word, &j);
        else
            new_expansion = expand_plain_text(current_word, &j);

        char *temp_word = ft_strjoin(expanded_word, new_expansion);
        // free(expanded_word);
        // free(new_expansion);
        expanded_word = temp_word;
    }
    return expanded_word;
}
