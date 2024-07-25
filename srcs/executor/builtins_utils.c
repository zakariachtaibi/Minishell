/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:05:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/25 12:08:06 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
        printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }

    // Free the copied list
    while (copy) {
        t_env_var *next = copy->next;
        free(copy->key);
        free(copy->value);
        free(copy);
        copy = next;
    }
}
