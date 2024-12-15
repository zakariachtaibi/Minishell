/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:09:34 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 22:29:55 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_plain_text(const char *current_word, size_t *j, int *flag)
{
	*flag = 2;
	return (expand_plain_text(current_word, j));
}

void	append_expansion(char **expanded_word, char *new_expansion)
{
	char	*temp_word;

	temp_word = ft_strjoin(*expanded_word, new_expansion);
	free(*expanded_word);
	free(new_expansion);
	*expanded_word = temp_word;
}

char	*expand_vars(t_tools *tools, char *current_word)
{
	size_t	j;
	size_t	len;
	char	*expanded_word;
	char	*new_expansion;

	j = 0;
	len = ft_strlen(current_word);
	expanded_word = NULL;
	while (j < len)
	{
		new_expansion = NULL;
		if (current_word[j] == '\'')
			new_expansion = handle_single_quote(current_word, &j, &tools->flag);
		else if (current_word[j] == '"')
			new_expansion = handle_double_quote(tools, current_word, &j);
		else if (current_word[j] == '$')
			new_expansion = handle_dollar_sign(tools, current_word, &j, len);
		else
			new_expansion = handle_plain_text(current_word, &j, &tools->flag);
		if (new_expansion)
			append_expansion(&expanded_word, new_expansion);
	}
	return (expanded_word);
}

t_env_var	*create_env_var(const char *env_entry)
{
	t_env_var	*new_var;
	char		*equal_sign;

	equal_sign = ft_strchr(env_entry, '=');
	if (!equal_sign)
		return (NULL);
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return (NULL);
	new_var->key = ft_substr(env_entry, 0, equal_sign - env_entry);
	new_var->value = ft_strdup(equal_sign + 1);
	new_var->next = NULL;
	if (!new_var->key || !new_var->value)
	{
		free(new_var->key);
		free(new_var->value);
		free(new_var);
		return (NULL);
	}
	return (new_var);
}

void	append_env_var(t_env_var **head, t_env_var *new_var)
{
	t_env_var	*current;

	if (*head == NULL)
		*head = new_var;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_var;
	}
}
