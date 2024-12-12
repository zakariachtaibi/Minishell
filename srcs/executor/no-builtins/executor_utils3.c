/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:04:43 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/12/11 15:58:48 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	prints_error(char *cmd, char *msg, int *exit_status)
{
	write(2, "minishell: ", 12);
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	*exit_status = 126;
}

static void	cleanup_and_exit(char **env_array, t_lexical *tokens,
		t_tools *tools, t_simple_cmds *cmd)
{
	ft_free(env_array);
	free_lexical(tokens);
	free_tools(tools);
	free_cmds(&cmd);
	perror("execve");
	exit(126);
}

int	execute_command(t_simple_cmds *cmd, t_tools **tools,
		t_lexical *tokens)
{
	pid_t	pid;
	int		status;
	char	**env_array;

	pid = fork();
	if (pid == 0)
	{
		env_array = convert_env_vars_to_array((*tools)->env_vars);
		execve(cmd->str[0], cmd->str, env_array);
		cleanup_and_exit(env_array, tokens, *tools, cmd);
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

void	handle_special_commands(t_simple_cmds *cmd,
		t_tools **tools, int *should_return)
{
	*should_return = 1;
	if (ft_strcmp(cmd->str[0], "..") == 0)
	{
		write(2, "minishell: ..: command not found\n", 34);
		(*tools)->exit_status = 127;
	}
	else if (ft_strcmp(cmd->str[0], "minishell") == 0)
	{
		write(2, "minishell: command not found\n", 30);
		(*tools)->exit_status = 127;
	}
	else if (cmd->str[0][0] == '\0')
	{
		write(2, "minishell: command not found\n", 30);
		(*tools)->exit_status = 127;
	}
	else
		*should_return = 0;
}

void	execute(char *cmd_path, t_simple_cmds *current_cmd, t_tools **tools)
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
