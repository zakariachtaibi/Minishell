/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/10 15:26:48 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	char	*buff;

	(void)cmd;
	(void)tokens;
	buff = malloc(1024 * sizeof(char));
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
	ft_putstr_fd(buff, 1);
	printf("\n");
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
		cleanup_readline();
        if (tokens)
            free_lexical(tokens);
        free_cmds(&cmd);
        free_tools(tools);
		exit(exit_status);
	}
	if (!is_numeric(cmd->str[1]))
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->str[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_readline();
        if (tokens)
            free_lexical(tokens);
        free_cmds(&cmd);
        free_tools(tools);
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
	int			i;
	int			had_error;
	t_env_var	*current;
	t_env_var	*node_to_delete;
	(void)tokens;
	
	i = 1;
	had_error = 0;
	while (cmd->str[i])
	{
		if (!is_valid_identifier(cmd->str[i]))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
				cmd->str[i]);
			had_error = 1;
			i++;
			continue ;
		}
		current = tools->env_vars;
		node_to_delete = NULL;
		while (current)
		{
			if (strcmp(current->key, cmd->str[i]) == 0)
			{
				node_to_delete = current;
				break ;
			}
			current = current->next;
		}
		if (node_to_delete)
		{
			delete_node_env(&(tools->env_vars), node_to_delete);
			if (tools->exit_status == 1)
				had_error = 1;
		}
		i++;
	}
	if (had_error)
		tools->exit_status = 1;
	else
		tools->exit_status = 0;
	return (tools->exit_status);
}

int	builtin_env(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	int			i;
	t_env_var	*current;
	(void)tokens;

	i = 1;
	current = tools->env_vars;
	if (cmd->str[i] == NULL)
	{
		if (!current)
		{
			printf("No environment variables set.\n");
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
	else
	{
		ft_putstr_fd("env: ", 2);
		ft_putstr_fd(cmd->str[i], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		tools->exit_status = 1;
	}
	return (1);
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
		if (strcmp(cmd->str[j], "~") == 0)
			ft_putstr_fd("/Users/hchouai", 1);
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
