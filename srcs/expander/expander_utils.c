/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:55:58 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/06 16:51:53 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_env_vars(t_tools *tools, char **envp)
{
	t_env_var	*head;
	char		*equal_sign;
	t_env_var	*new_var;

	tools->env_vars = NULL;
	head = NULL;
	while (*envp)
	{
		equal_sign = ft_strchr(*envp, '=');
		if (equal_sign)
		{
			new_var = malloc(sizeof(t_env_var));
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
