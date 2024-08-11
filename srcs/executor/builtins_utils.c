/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:05:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/09 13:58:20 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void search_for_argn(t_simple_cmds *cmd, int *flag, int *j)
{
    int i;

    while (cmd->str[*j] && cmd->str[*j][0] == '-')
    {
        i = 1;
        if (cmd->str[*j][1] == '\0')
            break;
        while (cmd->str[*j][i] == 'n')
            i++;
        if (cmd->str[*j][i] == '\0')
        {
            *flag = 1;
            *j += 1;
        }
        else
            break;
    }
}

void	delete_node_env(t_env_var **head, t_env_var *node_to_delete)
{
	if (node_to_delete == NULL)
		return ;
	if (*head == node_to_delete)
		*head = node_to_delete->next;
    else
    {
        t_env_var *current = *head;
        while (current != NULL && current->next != node_to_delete)
            current = current->next;
        if (current != NULL)
            current->next = node_to_delete->next;
    }
	free(node_to_delete->key);
    free(node_to_delete->value);
	free(node_to_delete);
}

void print_sorted_env(t_tools *tools)
{
    // Create a copy of the environment list
    t_env_var *copy;
    t_env_var *temp;

    copy = NULL;
    temp = tools->env_vars;
    while (temp)
    {
        t_env_var *new_node;
        
        new_node= malloc(sizeof(t_env_var));
        new_node->key = ft_strdup(temp->key);
        new_node->value = ft_strdup(temp->value);
        new_node->next = copy;
        copy = new_node;
        temp = temp->next;
    }

    // Sort the copied list using while loop
    t_env_var *i = copy;
    while (i)
    {
        t_env_var *j = i->next;
        while (j) {
            if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0) {
                // Swap key and value
                char *temp_key = i->key;
                char *temp_value = i->value;
                i->key = j->key;
                i->value = j->value;
                j->key = temp_key;
                j->value = temp_value;
            }
            j = j->next;
        }
        i = i->next;
    }

    // Print the sorted list
    t_env_var *current = copy;
    while (current) {
        if (!ft_strncmp(current->value, "", 1))
            printf("declare -x %s\n", current->key);
        else
            printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }

    // Free the copied list
    while (copy) {
        t_env_var *next = copy->next;
        free(copy->key);
        free(copy->value);
        free(copy);
        copy = next;    }
}

char *get_env_value(t_env_var *env_vars, const char *key)
{
    size_t key_len;
    
    key_len = ft_strlen(key);
    while (env_vars)
    {
        if (!ft_strncmp(env_vars->key, key, key_len) && env_vars->key[key_len] == '\0')
            return (env_vars->value);
        env_vars = env_vars->next;
    }
    return NULL;
}

void add_new_env_var(t_tools *tools, char *key, char *value, t_env_var *current, t_env_var *prev)
{
    t_env_var *new_var;

    new_var = malloc(sizeof(t_env_var));
    new_var->key = key;
    new_var->value = value;
    new_var->next = current;
    if (prev)
        prev->next = new_var;
    else
        tools->env_vars = new_var;
}

void handle_env_var(t_tools *tools, char *key, char *value)
{
    t_env_var *current;
    t_env_var *prev;

    current = tools->env_vars;
    prev = NULL;
    while (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) < 0)
    {
        prev = current;
        current = current->next;
    }

    if (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
    {
        free(current->value);
        current->value = value;
        free(key);
    }
    else
        add_new_env_var(tools, key, value, current, prev);
}

int is_numeric(char *str)
{
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
