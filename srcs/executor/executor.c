/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/11 13:07:55 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **convert_env_vars_to_array(t_env_var *env_vars)
{
    int         count;
    t_env_var   *temp;
    int         i;
    char        *temp_value;
    char        **envp;

    count = 0;
    temp = env_vars;
    i = -1;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    envp = malloc((count + 1) * sizeof(char *));
    if (!envp)
        return NULL;

    temp = env_vars;
    while (++i < count)
    {
        envp[i] = ft_strjoin(temp->key, "=");
        temp_value = envp[i];
        envp[i] = ft_strjoin(envp[i], temp->value);
        free(temp_value);
        temp = temp->next;
    }
    envp[count] = NULL;
    return (envp);
}

void execute_cmd(t_simple_cmds *current_cmd, t_tools **tools)
{
    t_env_var *env_var;
    char **split;
    char *full_cmd;
    char *cmd_path;

    split = NULL;
    full_cmd = NULL;
    cmd_path = NULL;
    if (access(current_cmd->str[0], X_OK) == 0)
    {
        pid_t pid = fork();
        int status;

        if (pid == 0)
        {
            execve(current_cmd->str[0], current_cmd->str, convert_env_vars_to_array((*tools)->env_vars));
            perror("execve");
            exit(1);
        }
        else if (pid < 0)
            perror("fork");
        else
        {
            waitpid(pid, &status, 0);
            (*tools)->exit_status = WEXITSTATUS(status);
        }
        return;
    }
    env_var = (*tools)->env_vars;
    while (env_var)
    {
        if (!ft_strncmp(env_var->key, "PATH", 5))
        {
            split = ft_split(env_var->value, ':');
            break;
        }
        env_var = env_var->next;
    }
    if (split)
    {
        int i = 0;
        while (split[i])
        {
            full_cmd = ft_strjoin(split[i], "/");
            cmd_path = ft_strjoin(full_cmd, current_cmd->str[0]);
            free(full_cmd);

            if (access(cmd_path, X_OK) == 0)
            {
                pid_t pid;
                int status;

                pid = fork();
                if (pid == 0)
                {
                    execve(cmd_path, current_cmd->str, convert_env_vars_to_array((*tools)->env_vars));
                    perror("execve");
                    exit(1);
                }
                else if (pid < 0)
                    perror("fork");
                else
                {
                    waitpid(pid, &status, 0);
                    (*tools)->exit_status = WEXITSTATUS(status);
                }
                free(cmd_path);
                return;
            }
            free(cmd_path);
            i++;
        }
    }
    ft_putstr_fd(current_cmd->str[0], 2);
    ft_putstr_fd(": command not found\n", 2);
}

void execute_commands(t_simple_cmds *cmds_head, t_tools **tools)
{
    t_simple_cmds *current_cmd;

    current_cmd = cmds_head;
    while (current_cmd != NULL)
    {
        if (current_cmd->builtin != NULL) 
            current_cmd->builtin(*tools, current_cmd);
        else
            execute_cmd(current_cmd, tools);
        current_cmd = current_cmd->next;
    }
}

