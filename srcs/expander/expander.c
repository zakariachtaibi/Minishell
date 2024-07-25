/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/25 12:06:48 by hchouai          ###   ########.fr       */
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

char	*expand_vars(t_tools *tools, t_lexical *temp)
{
	size_t		j;
	size_t		var_start;
	size_t		var_len;
	char		*var_name;
	char		*var_value;
	char		temp_str[2];
	char		*expanded_word;
	char		*current_word;
	size_t		len;
	current_word = temp->str;
	expanded_word = ft_strdup("");
	len = ft_strlen(current_word);
	j = 0;
		while (j < len)
		{
        if (current_word[j] == '\'')
		{
            j++;  
            while (j < len && current_word[j] != '\'')
			{
                temp_str[0] = current_word[j];
                temp_str[1] = '\0';
                expanded_word = ft_strjoin(expanded_word, temp_str);
                j++;
            }
            j++; 
        } 
		else if (current_word[j] == '"') 
		{
            j++; 
            while (j < len && current_word[j] != '"')
			{
                if (current_word[j] == '$')
				{
                    j++;
                    var_start = j;
                    while (j < len && (ft_isalnum(current_word[j]) || current_word[j] == '_'))
                        j++;
                    var_len = j - var_start;
                    var_name = ft_strndup(&current_word[var_start], var_len);
                    var_value = get_vars_value(var_name, tools);
                    free(var_name);
                    expanded_word = ft_strjoin(expanded_word, var_value);
                    // free(var_value);
                } 
				else
				{
                    temp_str[0] = current_word[j];
                    temp_str[1] = '\0';
                    expanded_word = ft_strjoin(expanded_word, temp_str);
                    j++;
                }
            }
            j++; 
        } 
		else 
		{
            if (current_word[j] == '$')
			{
                j++;
                var_start = j;
                while (j < len && (ft_isalnum(current_word[j]) || current_word[j] == '_')) 
				{
                    j++;
                }
                var_len = j - var_start;
                var_name = ft_strndup(&current_word[var_start], var_len);
                var_value = get_vars_value(var_name, tools);
                free(var_name);
                expanded_word = ft_strjoin(expanded_word, var_value);
            }
			else
			{
                temp_str[0] = current_word[j];
                temp_str[1] = '\0';
                expanded_word = ft_strjoin(expanded_word, temp_str);
                j++;
            }
        }
    }
    return expanded_word;
}
