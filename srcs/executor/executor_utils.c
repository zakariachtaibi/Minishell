/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:26:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/21 10:27:16 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int count_env_vars(t_env_var *env_vars)
{
    int count = 0;
    t_env_var *temp = env_vars;

    while (temp)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

char **allocate_envp_array(int count)
{
    char **envp = malloc((count + 1) * sizeof(char *));
    if (!envp)
        return NULL;
    return envp;
}

void fill_envp_array(char **envp, t_env_var *env_vars, int count)
{
    t_env_var *temp = env_vars;
    int i = 0;

    while (i < count)
    {
        char *temp_value;
        envp[i] = ft_strjoin(temp->key, "=");
        temp_value = envp[i];
        envp[i] = ft_strjoin(envp[i], temp->value);
        free(temp_value);
        temp = temp->next;
        i++;
    }
    envp[count] = NULL;
}