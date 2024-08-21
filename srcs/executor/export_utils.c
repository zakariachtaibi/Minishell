/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:59:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/20 17:22:53 by hchouai          ###   ########.fr       */
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
    *value=ft_strdup(" ");
    *value = ft_strdup(equal_sign + 1);
}

// void concatenate_values(char **str, int *i, char **value)
// {
//     char *temp;

//     while (str[*i + 1] && (str[*i + 1][0] != '=' && str[*i + 1][0] != '-' && str[*i + 1][0] != '+'))
//     {
//         temp = *value;
//         *value = ft_strjoin(*value, " ");
//         free(temp);

//         temp = *value;
//         *value = ft_strjoin(*value, str[++(*i)]);
//         free(temp);
//     }
// }

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
        value = ft_strdup("");
    }
    while(key[in] != '\0')
    {
        if(!(ft_isalpha(key[in])))
        {
            printf("minishell : export: not a valid identifier\n");
            return ;
        }
        in++;
    }
    // concatenate_values(str, i, &value);
    handle_env_var(tools, key, value);
}