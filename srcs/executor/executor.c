/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/14 11:10:31 by zchtaibi         ###   ########.fr       */
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
            execve(current_cmd->str[0], current_cmd->str, NULL);
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
        perror("fork");
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
void execute_commands(t_simple_cmds *cmds_head, t_tools **tools, t_lexical *tokens)
{
    int             fd[2];
    pid_t           pid;
    int             in_fd;
    int             status;
    int             has_pipe;
    t_lexical       *curr_token;
    t_simple_cmds   *current_cmd;

    in_fd = 0;
    has_pipe = 0;
    curr_token = tokens;
    while (curr_token)
    {
        if (curr_token->token == TOKEN_PIPE)
        {
            has_pipe = 1;
            break;
        }
        curr_token = curr_token->next;
    }
    current_cmd = cmds_head;
    if (has_pipe)
    {
        // dup2((*tools)->std_out, 1);
		// dup2((*tools)->std_in, 0);
        while (current_cmd)
        {
            if (current_cmd->next != NULL)
            {
                if (pipe(fd) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                pid = fork();
                if (pid == 0)
                {
                    if (in_fd != 0)
                        dup2(in_fd, STDIN_FILENO);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    if (current_cmd->builtin != NULL)
                        current_cmd->builtin(*tools, current_cmd);
                    else
                        execute_cmd(current_cmd, tools);
                    exit(0);
                }
                else if (pid < 0)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                close(fd[1]);
                in_fd = fd[0];
                waitpid(pid, &status, 0);
                (*tools)->exit_status = WEXITSTATUS(status);
                current_cmd = current_cmd->next;
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    if (in_fd != 0)
                        dup2(in_fd, STDIN_FILENO);
                    if (current_cmd->builtin != NULL)
                        current_cmd->builtin(*tools, current_cmd);
                    else
                         execute_cmd(current_cmd, tools);
                    exit(0);
                }
                else if (pid < 0)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                waitpid(pid, &status, 0);
                (*tools)->exit_status = WEXITSTATUS(status);
                current_cmd = current_cmd->next;
            }
        }
    }
    else
    {
        while (current_cmd)
        {
            if (current_cmd->builtin != NULL)
                current_cmd->builtin(*tools, current_cmd);
            else
                execute_cmd(current_cmd, tools);
            current_cmd = current_cmd->next;
        }
    }
}






