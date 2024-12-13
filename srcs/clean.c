/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:55:22 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 02:11:02 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_readline(void)
{
	clear_history();
	rl_free_line_state();
}

void	free_env_var(t_env_var *env_vars)
{
	t_env_var	*tmp;

	while (env_vars)
	{
		tmp = env_vars;
		env_vars = env_vars->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
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
		if (current_env->key)
			free(current_env->key);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		current_env = next_env;
	}
	if (tools->var_name)
		free(tools->var_name);
	if (tools->var_value)
		free(tools->var_value);
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
	int				i;

	if (!cmds || !*cmds)
		return ;
	current = *cmds;
	while (current)
	{
		next = current->next;
		if (current->str)
		{
			i = 0;
			while (current->str[i])
			{
				free(current->str[i]);
				i++;
			}
			free(current->str);
		}
		if (current->redirections)
			free_lexical(current->redirections);
		if (current->hd_file_name)
			free(current->hd_file_name);
		free(current);
		current = next;
	}
	*cmds = NULL;
}
