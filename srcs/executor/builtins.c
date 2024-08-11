/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/11 11:28:18 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

    // if (has_unclosed_quotes_or_parentheses(cmd->str[i]))
    // {
    //     tools->exit_status = 1;
    //     return (1);
    // }
    // int j = 0;
    // while (cmd->str[i][j])
    // {
    //     if (!ft_isalnum(cmd->str[i][j]))
    //     {
    //         printf("minishell: export: %s: not a valid identifier\n", cmd->str[i]);
    //         tools->exit_status = 1;
    //         return (1);
    //     }
    //     j++;
    // }

int builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
    int     i;
    char    *key;
    char    *value;
    char    *existing_value;
    int     flag;
    char    *temp;

    i = 1;
    if (!cmd->str[i])
    {
        print_sorted_env(tools);
        tools->exit_status = 0;
        return (0);
    }
    while (cmd->str[i])
    {
        flag = 0;
        char *plus_equal_sign = ft_strnstr(cmd->str[i], "+=", ft_strlen(cmd->str[i]));
        if (plus_equal_sign)
        {
            flag = 1;
            key = ft_substr(cmd->str[i], 0, plus_equal_sign - cmd->str[i]);
            value = ft_strdup(plus_equal_sign + 2);
            existing_value = get_env_value(tools->env_vars, key);
            if (existing_value)
            {
                temp = ft_strjoin(existing_value, value);
                free(value);
                value = temp;
            }
        }
        else
        {
            char *equal_sign = ft_strchr(cmd->str[i], '=');
            if (equal_sign)
            {
                key = ft_substr(cmd->str[i], 0, equal_sign - cmd->str[i]);
                value = ft_strdup(equal_sign + 1);
            }
            else
            {
                key = ft_substr(cmd->str[i], 0, ft_strlen(cmd->str[i]));
                value = ft_strdup("");
            }
        }
        while (cmd->str[i + 1] && (cmd->str[i + 1][0] != '=' && cmd->str[i + 1][0] != '-' && cmd->str[i + 1][0] != '+'))
        {
            temp = value;
            value = ft_strjoin(value, " ");
            free(temp);
            temp = value;
            value = ft_strjoin(value, cmd->str[++i]);
            free(temp);
        }
        handle_env_var(tools, key, value);
        i++;
    }
    tools->exit_status = 0;
    return (0);
}


int builtin_pwd(t_tools *tools, t_simple_cmds *cmd)
{
    (void)cmd;
    char *buff;
    buff= malloc(1024 * sizeof(char));
    if (!buff)
    {
        perror("malloc");
        tools->exit_status = 1;
        return (1);
    }
    if (getcwd(buff, 1024) == NULL)
    {
        perror("getcwd");
        free(buff);
        tools->exit_status = 1;
        return (1);
    }
    printf("%s\n", buff);
    tools->working_dir_path = buff;
    free(buff);
    tools->exit_status = 0;
    return (0); 
}


int builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
    int exit_status;

    if (cmd->str[1] == NULL)
        exit(tools->exit_status);
    if (!is_numeric(cmd->str[1]))
    {
        ft_putstr_fd("exit\n", 2);
        printf("minishell: exit: %s: numeric argument required\n", cmd->str[1]);
        exit(2);
    }
    if (cmd->str[2] != NULL)
    {
        ft_putstr_fd("exit\n", 2);
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        tools->exit_status = 1;
        return (1);
    }
    exit_status = atoi(cmd->str[1]);
    exit(exit_status);
}

int builtin_unset(t_tools *tools, t_simple_cmds *cmd)
{
    t_env_var *current = tools->env_vars;
    int i = 1;
    
    while(cmd->str[i])
    {
        current =  tools->env_vars;
        while(current)
        {
            if(!(strcmp(current->key,cmd->str[i])))
            {
                delete_node_env(&(tools->env_vars), current);
               break ;
            }
           current = current->next; 
        }
        i++;
    }
    return(0);
}

int builtin_env(t_tools *tools, t_simple_cmds *cmd)
{
    int i;

    i = 1;
    if (cmd->str[i] == NULL)
    {
        t_env_var *current = tools->env_vars;
        while (current)
        {
            if (ft_strncmp(current->value, "", 1))
                printf("%s=%s\n", current->key, current->value);
            current = current->next;
        }
        return (0);
    }
    return (1);
}

int	builtin_echo(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
	int j;
    int flag;
    
    j = 1;
    flag = 0;
    search_for_argn(cmd, &flag, &j);
	while(cmd->str[j])
	{
		if (!ft_strncmp(cmd->str[j], "?", 2))
        {
            char *exit_status_str = ft_itoa(tools->exit_status);
            ft_putstr_fd(exit_status_str, 1);
            free(exit_status_str);
        }
        else
            ft_putstr_fd(cmd->str[j], cmd->fd_out);

		if (cmd->str[j + 1] != NULL)
            ft_putchar_fd(' ',1);
		j++;
	}
	if (!flag)
        ft_putchar_fd('\n',1);
    tools->exit_status = 0;
    return (0);
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd)
{
    t_env_var *current;
    t_env_var *old_pwd;

    if (cmd->str[2] != NULL)
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        tools->exit_status = 1;
        return (1);
    }
    if (cmd->str[1] == NULL)
    {
        cmd->str[1] = get_env_value(tools->env_vars, "HOME");
        if (cmd->str[1] == NULL)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            tools->exit_status = 1;
            return (1);
        }        
    }
    if(chdir(cmd->str[1]) != 0)
    {
        printf("cd: %s: No such file or directory\n", cmd->str[1]);
        tools->exit_status = 1;
        return (1);
    }
    char *new_pwd  = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("getcwd");
        tools->exit_status = 1;
        return (1);
    }
    current = tools->env_vars;
    old_pwd = NULL;
    while (current)
    {
        if (!ft_strncmp(current->key, "PWD", 3))
        {
            t_env_var *tmp = current;
            current = tools->env_vars;
            while (current)
            {
                if (!ft_strncmp(current->key, "OLDPWD", 3))
                {
                    free(current->value);
                    current->value = ft_strdup(tmp->value);
                    break;
                }
                current= current->next;
            }
            free(tmp->value);
            tmp->value = new_pwd;
            break;
        }
        current = current->next;
    }
    tools->exit_status = 0;
    return (0);
}