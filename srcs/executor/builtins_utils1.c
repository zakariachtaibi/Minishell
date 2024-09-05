/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:42:32 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/03 10:58:45 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void sort_env_vars(t_env_var *copy)
{
    t_env_var *i = copy;

    while (i)
    {
        t_env_var *j = i->next;
        while (j)
        {
            if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0)
            {
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
}

void print_env_vars(t_env_var *copy)
{
    t_env_var *current = copy;

    while (current)
    {
        // if (current->value[0] == '\0')
        //     printf("declare -x %s=\"\"\n", current->key);
         if (current->value == NULL)
            printf("declare -x %s\n", current->key);
        // else if (current->value[0] == '\0')  // Case when the value is an empty string `""` (e.g., export hello="")
        //     printf("declare -x %s=\"\"\n", current->key);
        else
        {
            if (current->value[0] == '\0')
                printf("declare -x %s\n", current->key);
            else
                printf("declare -x %s=\"%s\"\n", current->key, current->value);
        } // General case where value is non-empty

        current = current->next;
    }
}


void free_env_vars(t_env_var *copy)
{
    while (copy)
    {
        t_env_var *next = copy->next;
        free(copy->key);
        free(copy->value);
        free(copy);
        copy = next;
    }
}

char *get_env_value(t_env_var *env_vars, const char *key)
{
    size_t key_len;
    
    key_len = ft_strlen(key);
    while (env_vars)
    {
        // Check if the current node's key matches the provided key
        if (!ft_strncmp(env_vars->key, key, key_len) && env_vars->key[key_len] == '\0')
            return (env_vars->value);
        env_vars = env_vars->next;
    }
    return NULL;
}


int is_numeric(char *str)
{
    int i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}