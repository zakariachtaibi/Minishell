/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:05:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/12 14:06:12 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	search_for_argn(t_simple_cmds *cmd, int *flag, int *j)
{
	int	i;

	while (cmd->str[*j] && cmd->str[*j][0] == '-')
	{
		i = 1;
		if (cmd->str[*j][1] == '\0')
			break ;
		while (cmd->str[*j][i] == 'n')
			i++;
		if (cmd->str[*j][i] == '\0')
		{
			*flag = 1;
			*j += 1;
		}
		else
			break ;
	}
}

void	delete_node_env(t_env_var **head, t_env_var *node_to_delete)
{
	t_env_var	*current;

	if (node_to_delete == NULL)
		return ;
	if (*head == node_to_delete)
		*head = node_to_delete->next;
	else
	{
		current = *head;
		while (current != NULL && current->next != node_to_delete)
			current = current->next;
		if (current != NULL)
			current->next = node_to_delete->next;
	}
	free(node_to_delete->key);
	free(node_to_delete->value);
	free(node_to_delete);
}

t_env_var	*copy_env_vars(t_env_var *env_vars)
{
	t_env_var	*copy;
	t_env_var	*temp;
	t_env_var	*new_node;

	copy = NULL;
	temp = env_vars;
	while (temp)
	{
		new_node = malloc(sizeof(t_env_var));
		new_node->key = ft_strdup(temp->key);
		if (temp->value == NULL)
			new_node->value = NULL;
		else
			new_node->value = ft_strdup(temp->value);
		new_node->next = copy;
		copy = new_node;
		temp = temp->next;
	}
	return (copy);
}

void	add_new_env_var(t_tools *tools, char *key, char *value)
{
	t_env_var	*new_var;
	t_env_var	*current;
	t_env_var	*prev;

	current = tools->env_vars;
	prev = NULL;
	while (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) < 0)
	{
		prev = current;
		current = current->next;
	}
	new_var = malloc(sizeof(t_env_var));
	new_var->key = key;
	new_var->value = value;
	new_var->next = current;
	if (prev)
		prev->next = new_var;
	else
		tools->env_vars = new_var;
}

void	handle_env_var(t_tools **tools, char *key, char *value)
{
	t_env_var	*current;

	current = (*tools)->env_vars;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (!current->value)
				free(current->value);
			if (value)
				current->value = value;
			free(key);
			return ;
		}
		current = current->next;
	}
	add_new_env_var((*tools), key, value);
}
