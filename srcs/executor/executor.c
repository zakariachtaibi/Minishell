/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/22 20:47:37 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int execute_if_absolute_path(t_simple_cmds *current_cmd, t_tools **tools)
{
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
        {
            perror("fork");
        }
        else
        {
            waitpid(pid, &status, 0);
            (*tools)->exit_status = WEXITSTATUS(status);
        }
        return 1;
    }
    return 0;
}

void execute(char *cmd_path, t_simple_cmds *current_cmd, t_tools **tools)
{
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        execve(cmd_path, current_cmd->str, convert_env_vars_to_array((*tools)->env_vars));
        perror("execve");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        waitpid(pid, &status, 0);
        (*tools)->exit_status = WEXITSTATUS(status);
    }
}

int execute_from_path(char **split, t_simple_cmds *current_cmd, t_tools **tools)
{
    char *full_cmd;
    char *cmd_path;
    int i = 0;

    while (split[i])
    {
        full_cmd = ft_strjoin(split[i], "/");
        cmd_path = ft_strjoin(full_cmd, current_cmd->str[0]);
        free(full_cmd);

        if (access(cmd_path, X_OK) == 0)
        {
            execute(cmd_path, current_cmd, tools);
            free(cmd_path);
            return 1;
        }
        free(cmd_path);
        i++;
    }
    return 0;
}

void execute_cmd(t_simple_cmds *current_cmd, t_tools **tools)
{
    if (execute_if_absolute_path(current_cmd, tools))
        return;

    char **split = get_path_dirs((*tools)->env_vars);
    if (split && execute_from_path(split, current_cmd, tools))
        return;

    handle_command_not_found(current_cmd, tools);
}

void execute_commands(t_simple_cmds *cmds_head, t_tools **tools)
{
    t_simple_cmds *current_cmd;

    current_cmd = cmds_head;
    if (current_cmd->str == NULL)
        return ;
    while (current_cmd != NULL)
    {
        if (current_cmd->builtin != NULL) 
            current_cmd->builtin(*tools, current_cmd);
        else
            execute_cmd(current_cmd, tools);
        current_cmd = current_cmd->next;
    }
}

