/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:26:03 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/07/22 13:15:39 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void get_env_vars(t_tools *tools, char **envp)
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

// char *expand_vars(t_simple_cmds **cmds, t_tools *tools)
// {
// 	int i = 1;
// 	int j = 0;
// 	char *new_cmd;
// 	while(*cmds)
// 	{
// 		while((*cmds)->str[i][j] && (*cmds)->str[i][j] != '$')
// 			j++;
// 		if((*cmds)->str[i][j] == '$')
// 		{
// 			while(tools->env_vars)
// 			{
// 				// ft_strchr(tools->env_vars->key, )
// 			}
// 		}
		
// 	}
// }