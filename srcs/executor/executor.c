/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/10 02:38:42 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigint_child(int sig)
{
	(void)sig;
	printf("\n");
	exit(130);
}

void	handle_sigaquit_child(int test)
{
	(void)test;
	printf("Quit (core dumped)\n");
	exit(131);
}

void	setup_child_signals(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, handle_sigaquit_child);
}

int execute_if_absolute_path(t_simple_cmds *current_cmd, t_tools **tools, t_lexical *tokens)
{
    struct stat statbuf;
    pid_t pid;
    int status;
    char **env_array;

    if (stat(current_cmd->str[0], &statbuf) == 0)
    {
        if (S_ISDIR(statbuf.st_mode))
        {
            write(2, "minishell: ", 12);
            write(2, current_cmd->str[0], ft_strlen(current_cmd->str[0]));
            write(2, ": is a directory\n", 17);
            (*tools)->exit_status = 126;
            return (1);
        }
        if (access(current_cmd->str[0], X_OK) == 0)
        {
            pid = fork();
            if (pid == 0)
            {
                env_array = convert_env_vars_to_array((*tools)->env_vars);
                execve(current_cmd->str[0], current_cmd->str, env_array);
                ft_free(env_array);
                free_lexical(tokens);
                free_tools(*tools);
                free_cmds(&current_cmd);
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
            return (1);
        }
        else
        {
            write(2, "minishell: ", 12);
            write(2, current_cmd->str[0], ft_strlen(current_cmd->str[0]));
            write(2, ": Permission denied\n", 20);
            (*tools)->exit_status = 126;
            return (1);
        }
    }
    return (0);
}

void execute(char *cmd_path, t_simple_cmds *current_cmd, t_tools **tools)
{
    pid_t pid;
    int status;
    char **env_array;

    pid = fork();
    if (pid == 0)
    {
        env_array = convert_env_vars_to_array((*tools)->env_vars);
        execve(cmd_path, current_cmd->str, env_array);
        ft_free(env_array);
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
}

int	execute_from_path(char **split, t_simple_cmds *current_cmd, t_tools **tools)
{
	char	*full_cmd;
	char	*cmd_path;
	int		i;

	i = 0;
	while (split[i])
	{
		full_cmd = ft_strjoin(split[i], "/");
		cmd_path = ft_strjoin(full_cmd, current_cmd->str[0]);
		free(full_cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			execute(cmd_path, current_cmd, tools);
			free(cmd_path);
			return (1);
		}
		free(cmd_path);
		i++;
	}
	return (0);
}

void	execute_cmd(t_simple_cmds *current_cmd, t_tools **tools, t_lexical *tokens)
{
	char	**split;

	if (!current_cmd || !current_cmd->str || !current_cmd->str[0])
	{
		(*tools)->exit_status = 0;  // Set exit status to 0 for pure redirections
		return;
	}

	if (ft_strcmp(current_cmd->str[0], ".") == 0)
	{
		if (!current_cmd->str[1])
		{
			write(2, "bash: .: filename argument required\n", 37);
			write(2, ".: usage: . filename [arguments]\n", 33);
			(*tools)->exit_status = 2;
			return ;
		}
	}
	if (ft_strcmp(current_cmd->str[0], "..") == 0)
	{
		write(2, "bash: ..: command not found\n", 29);
		(*tools)->exit_status = 127;
		return ;
	}
	if(ft_strcmp(current_cmd->str[0],"minishell") == 0)
	{
		write(2, "bash: : command not found\n", 27);
		(*tools)->exit_status = 127;
		return ;
	}
	if (current_cmd->str[0][0] == '\0')
	{
		write(2, "bash: : command not found\n", 27);
		(*tools)->exit_status = 127;
		return ;
	}
	if (execute_if_absolute_path(current_cmd, tools, tokens))
		return ;
	split = get_path_dirs((*tools)->env_vars);
	if (split && execute_from_path(split, current_cmd, tools))
	{
		ft_free(split);
		return ;
	}
	handle_command_not_found(current_cmd, tools);
	if (split)
		ft_free(split);
}

int	count_cmds(t_simple_cmds *list)
{
	t_simple_cmds	*tmp;
	int				i;

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
    int pipe_fd[2];
    int prev_pipe_read = STDIN_FILENO;
    int status;
    t_simple_cmds *current_cmd;
    pid_t pid;
    pid_t last_pid = 0;
    int e;
    int builtin_executed = 0;

    status = 0;
    current_cmd = cmds_head;
    while (current_cmd)
    {       
        if (current_cmd->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        
        if (current_cmd->builtin != NULL && current_cmd->next == NULL && 
            prev_pipe_read == STDIN_FILENO)
        {
            int old_stdout = dup(STDOUT_FILENO);
            int old_stdin = dup(STDIN_FILENO);
            
            if (current_cmd->fd_in != STDIN_FILENO)
                dup2(current_cmd->fd_in, STDIN_FILENO);
            if (current_cmd->fd_out != STDOUT_FILENO)
                dup2(current_cmd->fd_out, STDOUT_FILENO);
            (*tools)->exit_status = current_cmd->builtin(*tools, current_cmd, tokens);
            builtin_executed = 1;
            dup2(old_stdout, STDOUT_FILENO);
            dup2(old_stdin, STDIN_FILENO);
            close(old_stdout);
            close(old_stdin);
            current_cmd = current_cmd->next;
            continue;
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

            if (current_cmd->fd_in != STDIN_FILENO)
            {
                dup2(current_cmd->fd_in, STDIN_FILENO);
                close(current_cmd->fd_in);
            }

            if (current_cmd->fd_out != STDOUT_FILENO)
            {
                dup2(current_cmd->fd_out, STDOUT_FILENO);
                close(current_cmd->fd_out);
            }

            if (current_cmd->builtin != NULL)
            {
                e = current_cmd->builtin(*tools, current_cmd, tokens);
                free_cmds(&cmds_head);
                free_tools((*tools));
				free_lexical(tokens);
                exit(e);
            }
            else
            {
                execute_cmd(current_cmd, tools, tokens);
                e = (*tools)->exit_status;
                free_cmds(&cmds_head);
                if (tokens)
                    free_lexical(tokens);
                free_tools((*tools));
                exit(e);
            }
        }
        else if (pid < 0)
        {
            perror("fork");
            free_cmds(&cmds_head);
            free_env_var((*tools)->env_vars);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (prev_pipe_read != STDIN_FILENO)
                close(prev_pipe_read);
            if (current_cmd->next)
            {
                close(pipe_fd[1]);
                prev_pipe_read = pipe_fd[0];
            }
            last_pid = pid;
        }
        current_cmd = current_cmd->next;
    }
    
    if (!builtin_executed)
    {
        waitpid(last_pid, &status, 0);
        if (WIFEXITED(status))
            (*tools)->exit_status = WEXITSTATUS(status);
    }
    
    while (wait(NULL) > 0)
        continue;
        
    if (cmds_head && cmds_head->next == NULL && cmds_head->fd_in != STDIN_FILENO)
        (*tools)->exit_status = 0;
}