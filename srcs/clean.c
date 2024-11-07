/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:55:22 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/07 02:07:44 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_readline(void)
{
	clear_history();
	rl_free_line_state(); 
}

void	free_tools(t_tools *tools)
{
	t_env_var	*current_env;
	t_env_var	*next_env;

	if (tools == NULL)
		return ;
	current_env = tools->env_vars;
	while (current_env)
	{
		next_env = current_env->next;
		free(current_env->key);
		free(current_env->value);
		free(current_env);
		current_env = next_env;
	}
	free(tools->var_name);
	free(tools->var_value);
	free(tools->working_dir_path);
	if (tools->std_out > 2)
		close(tools->std_out);
	if (tools->std_in > 2)
		close(tools->std_in);
	free(tools);
}


void	free_lexical(t_lexical *head)
{
	t_lexical	*current;
	t_lexical	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		if (current->str)
			free(current->str);
		free(current);
		current = next;
	}
}

void	free_cmds(t_simple_cmds **cmds)
{
	t_simple_cmds	*current;
	t_simple_cmds	*next;
	

	current = *cmds;
	while (current)
	{
		next = current->next;
		if (current->str)
		{
			for (int i = 0; current->str[i]; i++)
				free(current->str[i]);
			free(current->str);
		}
		if (current->hd_file_name)
			free(current->hd_file_name);
		free(current);
		current = next;
	}
}