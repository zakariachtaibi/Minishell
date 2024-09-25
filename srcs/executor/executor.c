/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/25 21:49:08 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_sigint_child(int sig)
{
    (void)sig;
    printf("\n");
    exit(130);
}

void handle_sigaquit_child(int test)
{
    (void)test;
    printf("Quit (core dumped)\n");
    exit(131);
}

void setup_child_signals(void)
{
    signal(SIGINT, handle_sigint_child);
    signal(SIGQUIT, handle_sigaquit_child);
}


int execute_if_absolute_path(t_simple_cmds *current_cmd, t_tools **tools)
{
    struct stat statbuf;

    if (stat(current_cmd->str[0], &statbuf) == 0)
    {
        if (S_ISDIR(statbuf.st_mode))
        {
            write(2, "minishell: ", 12);
            write(2, current_cmd->str[0], ft_strlen(current_cmd->str[0]));
            write(2, ": is a directory\n", 17);
            (*tools)->exit_status = 126;
            return 1;
        }
        else if (access(current_cmd->str[0], X_OK) == 0)
        {
            pid_t pid = fork();
            int status;

            if (pid == 0)
            {
                execve(current_cmd->str[0], current_cmd->str,  convert_env_vars_to_array((*tools)->env_vars));
                perror("execve");
                exit(126);
            }
            else if (pid < 0)
                perror("fork");
            else
            {
                waitpid(pid, &status, 0);
                (*tools)->exit_status = WEXITSTATUS(status);
            }
            return 1;
        }
    } 
    // else if (stat(current_cmd->str[0], &statbuf) == -1)
    // {
    //     write(2, "minishell: ", 12);
    //     write(2, current_cmd->str[0], ft_strlen(current_cmd->str[0]));
    //     write(2, ": No such file or directory\n", 28);
    //     (*tools)->exit_status = 126;
    //     return 1;
    // }
    return 0;
}

void execute(char *cmd_path, t_simple_cmds *current_cmd, t_tools **tools)
{
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        execve(cmd_path, current_cmd->str, convert_env_vars_to_array((*tools)->env_vars));
        // perror("execve"); why this should be here?
        // exit(126);
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

int count_cmds(t_simple_cmds *list)
{
    t_simple_cmds *tmp;
    int i;

    i = 0;
    tmp = list;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

void execute_commands(t_simple_cmds *cmds_head, t_tools **tools, t_lexical *tokens)
{
    int             pipe_fd[2];
    int             prev_pipe_read = STDIN_FILENO;
    int             status;
    t_simple_cmds   *current_cmd;
    pid_t           pid;
    (void)          tokens;

    // int exit_status = 0;
    current_cmd = cmds_head;

    while (current_cmd)
    {
        if (current_cmd->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if (current_cmd->builtin != NULL)
        {
            if (current_cmd->next == NULL && prev_pipe_read == STDIN_FILENO)
            {
                int old_stdout = dup(STDOUT_FILENO);
                int old_stdin = dup(STDIN_FILENO);
                if (current_cmd->fd_in != STDIN_FILENO)
                    dup2(current_cmd->fd_in, STDIN_FILENO);
                if (current_cmd->fd_out != STDOUT_FILENO)
                    dup2(current_cmd->fd_out, STDOUT_FILENO);
                (*tools)->exit_status = current_cmd->builtin(*tools, current_cmd);
                dup2(old_stdout, STDOUT_FILENO);
                dup2(old_stdin, STDIN_FILENO);
                close(old_stdout);
                close(old_stdin);
                current_cmd = current_cmd->next;
                continue;
            }
        }
        pid = fork();
        if (pid == 0)
        {
            setup_child_signals();
            if (prev_pipe_read != STDIN_FILENO)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }

            if (current_cmd->next)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            // Handle input redirection
            if (current_cmd->fd_in != STDIN_FILENO)
            {
                dup2(current_cmd->fd_in, STDIN_FILENO);
                close(current_cmd->fd_in);
            }

            // Handle output redirection
            if (current_cmd->fd_out != STDOUT_FILENO)
            {
                dup2(current_cmd->fd_out, STDOUT_FILENO);
                close(current_cmd->fd_out);
            }

            if (current_cmd->builtin != NULL)
                exit(current_cmd->builtin(*tools, current_cmd));
            else
                execute_cmd(current_cmd, tools);
            exit((*tools)->exit_status);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            if (prev_pipe_read != STDIN_FILENO)
                close(prev_pipe_read);

            if (current_cmd->next)
            {
                close(pipe_fd[1]);
                prev_pipe_read = pipe_fd[0];
            }
            else
            {
                // Wait for the last command to finish
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    (*tools)->exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    (*tools)->exit_status = WTERMSIG(status) + 128;
            }
        }
        current_cmd = current_cmd->next;
    }
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            (*tools)->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            (*tools)->exit_status = WTERMSIG(status) + 128;
    }
}