/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:20:05 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/22 00:32:48 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	execute_if_absolute_path(t_simple_cmds *current_cmd,
									t_tools **tools,
									t_lexical *tokens)
{
	struct stat	statbuf;
	pid_t		pid;
	int			status;
	char		**env_array;

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

static void	execute(char *cmd_path, t_simple_cmds *current_cmd, t_tools **tools)
{
	pid_t	pid;
	int		status;
	char	**env_array;

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

static int	execute_from_path(char **split, t_simple_cmds *current_cmd,
		t_tools **tools)
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

void	execute_cmd(t_simple_cmds *current_cmd, t_tools **tools,
		t_lexical *tokens)
{
	char	**split;

	if (!current_cmd || !current_cmd->str || !current_cmd->str[0])
	{
		(*tools)->exit_status = 0;
		return ;
	}
	if (ft_strcmp(current_cmd->str[0], ".") == 0)
	{
		if (!current_cmd->str[1])
		{
			write(2, "minishell: .: filename argument required\n", 42);
			write(2, ".: usage: . filename [arguments]\n", 33);
			(*tools)->exit_status = 2;
			return ;
		}
	}
	if (ft_strcmp(current_cmd->str[0], "..") == 0)
	{
		write(2, "minishell: ..: command not found\n", 34);
		(*tools)->exit_status = 127;
		return ;
	}
	if (ft_strcmp(current_cmd->str[0], "minishell") == 0)
	{
		write(2, "minishell: command not found\n", 30);
		(*tools)->exit_status = 127;
		return ;
	}
	if (current_cmd->str[0][0] == '\0')
	{
		write(2, "minishell: command not found\n", 30);
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
