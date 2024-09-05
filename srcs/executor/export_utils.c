/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:59:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/03 11:26:10 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_plus_equal(t_tools *tools, char *str, char **key, char **value)
{
    char *plus_equal_sign = ft_strnstr(str, "+=", ft_strlen(str));
    char *existing_value;

    *key = ft_substr(str, 0, plus_equal_sign - str);
    *value = ft_strdup(plus_equal_sign + 2);
    existing_value = get_env_value(tools->env_vars, *key);

    if (existing_value)
    {
        char *temp = ft_strjoin(existing_value, *value);
        free(*value);
        *value = temp;
    }
}

void handle_equal(char *str, char **key, char **value)
{
    char *equal_sign = ft_strchr(str, '=');

    *key = ft_substr(str, 0, equal_sign - str);
    if (*(equal_sign + 1) == '\0')
        *value = ft_strdup(" ");
    else
        *value = ft_strdup(equal_sign + 1);
}

void process_export(t_tools *tools, char **str, int *i)
{
    char *key;
    char *value;
    char *plus_equal_sign = ft_strnstr(str[*i], "+=", ft_strlen(str[*i]));
    char *equal_sign = ft_strchr(str[*i], '=');
    int in = 0;

    if (plus_equal_sign)
        handle_plus_equal(tools, str[*i], &key, &value);
    else if (equal_sign)
        handle_equal(str[*i], &key, &value);
    else
    {
        key = ft_substr(str[*i], 0, ft_strlen(str[*i]));
        value = NULL;
    }
    while(key[in] != '\0')
    {
        if(!(ft_isalpha(key[in])) || key[in] == 32)
        {
            printf("minishell : export: not a valid identifier\n");
            return ;
        }
        in++;
    }
    handle_env_var(tools, key, value);
}

void print_sorted_env(t_tools *tools)
{
    t_env_var *copy = copy_env_vars(tools->env_vars);
    sort_env_vars(copy);
    print_env_vars(copy);
    free_env_vars(copy);
}

int builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
    int i = 1;

    if (!cmd->str[i])
    {
        print_sorted_env(tools);
        tools->exit_status = 0;
        return 0;
    }
    while (cmd->str[i])
    {
        process_export(tools, cmd->str, &i);
        i++;
    }
    tools->exit_status = 0;
    return 0;
}