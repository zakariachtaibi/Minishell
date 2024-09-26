/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:55:58 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/18 20:20:25 by hchouai          ###   ########.fr       */
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
			if (!(strcmp("HOME", current->key)))
				return (current->value);
			current = current->next;
		}
	}
	current = tools->env_vars;
	while (current)
	{
		if (!(strcmp(str, current->key)))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
