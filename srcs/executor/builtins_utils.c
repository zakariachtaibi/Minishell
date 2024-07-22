/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:05:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/22 13:38:18 by hchouai          ###   ########.fr       */
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
        {
            current = current->next;
        }
        if (current != NULL)
        {
            current->next = node_to_delete->next;
        }
    }
	free(node_to_delete->key);
    free(node_to_delete->value);
	free(node_to_delete);
    printf("ode deleted");
}