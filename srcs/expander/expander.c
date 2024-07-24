/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/24 11:00:56 by hchouai          ###   ########.fr       */
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

char	*expand_vars(char *str, t_tools *tools)
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