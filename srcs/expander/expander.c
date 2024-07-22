/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/22 12:26:09 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void init_env_vars(t_tools *tools, char **envp)
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