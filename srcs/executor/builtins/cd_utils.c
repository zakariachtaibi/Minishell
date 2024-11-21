/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:11:49 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/22 00:21:16 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	execute_cd(t_tools *tools, char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		tools->exit_status = 1;
		return (1);
	}
	return (0);
}

int	check_cd_arguments(t_tools *tools, t_simple_cmds *cmd)
{
	char	*path;

	path = cmd->str[1];
	if (path == NULL || ft_strcmp(path, "~") == 0)
	{
		if (handle_home_path(tools, &path) != 0)
			return (1);
	}
	if (check_cd_arg_count(tools, cmd) != 0)
		return (1);
	if (ft_strcmp(path, "\0") == 0)
		return (0);
	return (execute_cd(tools, path));
}

t_env_var	*find_env_var(t_env_var *env_vars, const char *key)
{
	while (env_vars)
	{
		if (ft_strcmp(env_vars->key, key) == 0)
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
