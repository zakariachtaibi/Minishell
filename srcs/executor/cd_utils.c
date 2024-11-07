/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:11:49 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/07 01:58:52 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_cd_arguments(t_tools *tools, t_simple_cmds *cmd)
{
	char	*path;

	path = NULL;
	if (cmd->str[1] == NULL || strcmp(cmd->str[1], "~") == 0)
	{
		path = get_env_value(tools->env_vars, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			tools->exit_status = 1;
			return (1);
		}
	}
	else if (cmd->str[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		tools->exit_status = 1;
		return (1);
	}
	else
		path = cmd->str[1];
	if (strcmp(path, "\0") == 0)
	{
		tools->exit_status = 0;
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd:", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		tools->exit_status = 1;
		return (1);
	}
	return (0);
}

t_env_var	*find_env_var(t_env_var *env_vars, const char *key)
{
	while (env_vars)
	{
		if (strcmp(env_vars->key, key) == 0)
			return (env_vars);
		env_vars = env_vars->next;
	}
	return (NULL);
}

void	update_pwd_variables(t_tools *tools)
{
	char		*new_pwd;
	t_env_var	*pwd;
	t_env_var	*old_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("getcwd");
		tools->exit_status = 1;
		return ;
	}
	pwd = find_env_var(tools->env_vars, "PWD");
	old_pwd = find_env_var(tools->env_vars, "OLDPWD");
	if (pwd)
	{
		if (old_pwd)
			free(old_pwd->value);
		if (old_pwd)
			old_pwd->value = ft_strdup(pwd->value);
		free(pwd->value);
		pwd->value = new_pwd;
	}
}

int	builtin_cd(t_tools *tools, t_simple_cmds *cmd, t_lexical *tokens)
{
	(void)tokens;
	if (check_cd_arguments(tools, cmd))
		return (1);
	update_pwd_variables(tools);
	return (0);
}
