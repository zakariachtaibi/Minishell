/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:20:05 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/12/11 15:58:40 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	execute_if_absolute_path(t_simple_cmds *cmd,
		t_tools **tools, t_lexical *tokens)
{
	struct stat	statbuf;

	if (stat(cmd->str[0], &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			prints_error(cmd->str[0], ": is a directory\n",
				&(*tools)->exit_status);
			return (1);
		}
		if (access(cmd->str[0], X_OK) == 0)
			return (execute_command(cmd, tools, tokens));
		prints_error(cmd->str[0], ": Permission denied\n",
			&(*tools)->exit_status);
		return (1);
	}
	return (0);
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

static void	try_execute_command(t_simple_cmds *cmd,
		t_tools **tools, t_lexical *tokens)
{
	char	**split;

	if (execute_if_absolute_path(cmd, tools, tokens))
		return ;
	split = get_path_dirs((*tools)->env_vars);
	if (split && execute_from_path(split, cmd, tools))
	{
		ft_free(split);
		return ;
	}
	handle_command_not_found(cmd, tools);
	if (split)
		ft_free(split);
}

void	execute_cmd(t_simple_cmds *cmd, t_tools **tools,
		t_lexical *tokens)
{
	int	should_return;

	if (!cmd || !cmd->str || !cmd->str[0])
	{
		(*tools)->exit_status = 0;
		return ;
	}
	if (ft_strcmp(cmd->str[0], ".") == 0)
	{
		if (!cmd->str[1])
		{
			write(2, "minishell: .: filename argument required\n", 42);
			write(2, ".: usage: . filename [arguments]\n", 33);
			(*tools)->exit_status = 2;
		}
		return ;
	}
	handle_special_commands(cmd, tools, &should_return);
	if (should_return)
		return ;
	try_execute_command(cmd, tools, tokens);
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
