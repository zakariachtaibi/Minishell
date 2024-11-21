/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/21 15:47:17 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	builtin_pwd(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	char	*buff;

	(void)cmd;
	(void)tokens;
	buff = get_current_working_directory(tools);
	if (!buff)
		return (1);
	ft_putstr_fd(buff, 1);
	ft_putstr_fd("\n", 1);
	tools->working_dir_path = buff;
	free(buff);
	return (0);
}

int	builtin_exit(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	int	exit_status;

	(void)tokens;
	exit_status = tools->exit_status;
	if (cmd->str[1] == NULL)
	{
		perform_exit_cleanup(cmd, tokens, tools);
		exit(exit_status);
	}
	if (!is_numeric(cmd->str[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(cmd->str[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		perform_exit_cleanup(cmd, tokens, tools);
		exit(2);
	}
	if (cmd->str[2] != NULL)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_status = ft_atoi(cmd->str[1]);
	exit(exit_status);
}

int	builtin_unset(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	int	i;
	int	had_error;

	(void)tokens;
	i = 1;
	had_error = 0;
	while (cmd->str[i])
	{
		if (!validate_unset_identifier(cmd->str[i]))
		{
			had_error = 1;
			i++;
			continue ;
		}
		if (remove_env_variable(tools, cmd->str[i]))
			had_error = 1;
		i++;
	}
	tools->exit_status = had_error;
	return (tools->exit_status);
}

int	builtin_env(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	t_env_var	*current;

	(void)tokens;
	if (cmd->str[1] != NULL)
		return (env_error(tools, cmd->str[1]));
	current = tools->env_vars;
	if (!current)
	{
		ft_putstr_fd("No environment variables set.\n", 2);
		return (0);
	}
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
	return (0);
}

int	builtin_echo(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	int	j;
	int	flag;

	(void)*tools;
	(void)tokens;
	j = 1;
	flag = 0;
	search_for_argn(cmd, &flag, &j);
	while (cmd->str[j])
	{
		if (ft_strcmp(cmd->str[j], "~") == 0)
			ft_putstr_fd("/Users/zchtaibi", 1);
		else
			ft_putstr_fd(cmd->str[j], 1);
		if (cmd->str[j + 1] != NULL)
			ft_putchar_fd(' ', 1);
		j++;
	}
	if (!flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
