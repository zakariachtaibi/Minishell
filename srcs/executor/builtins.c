/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/02 21:57:53 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
    int i;
    char *key;
    char *value;

    i = 1;
    if (!cmd->str[i])
    {
        print_sorted_env(tools);
        return (0);
    }
    while (cmd->str[i])
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
        handle_env_var(tools, key, value);
        i++;
    }
    return (0);
}

int builtin_pwd(t_tools *tools, t_simple_cmds *cmd)
{
    (void)cmd;
    (void)tools;
    char *buff;
    buff= malloc(1024 * sizeof(char));
    if (!buff)
    {
        perror("malloc");
        return (1);
    }
    if (getcwd(buff, 1024) == NULL)
    {
        perror("getcwd");
        free(buff);
        return (1);
    }
    printf("%s\n", buff);
    tools->working_dir_path = buff;
    free(buff);
    return (0); 
}

int builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
    (void)tools;

    int exit_status = 0;
    if (cmd->str[1] != NULL)
    {
        printf("exit\n");
        printf("minishell: exit: %s: numeric argument required\n", cmd->str[1]);
        exit_status = 1;
    }
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
    if (!ft_strncmp(cmd->str[j], "-n", 3))
    {
        j++;
        flag = 1;
    }
	while(cmd->str[j])
	{
		ft_putstr_fd(cmd->str[j],1);
		if (cmd->str[j + 1] != NULL)
            ft_putchar_fd(' ',1);
		j++;
	}
    
	if (!flag)
        ft_putchar_fd('\n',1);
    return (0);
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd)
{
    t_env_var *current;
    t_env_var *old_pwd;

    if (cmd->str[2] != NULL)
    {
        printf("minishell: cd: too many arguments\n");
        return (1);
    }
    if (cmd->str[1] == NULL)
    {
        cmd->str[1] = get_env_value(tools->env_vars, "HOME");
        if (cmd->str[1] == NULL)
        {
            write(2, "cd: HOME not set\n", 17);
            return 1;
        }        
    }
    if(chdir(cmd->str[1]) != 0)
    {
        printf("cd: %s: No such file or directory\n", cmd->str[1]);
        return (1);
    }
    char *new_pwd  = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("getcwd");
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
    return (0);
}