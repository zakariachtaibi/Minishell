/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 00:19:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/11 16:00:09 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_error(char *str, char *key, char *value, t_tools *tools)
{
	ft_putstr_fd("minishell: export `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(key);
	if (value)
		free(value);
	tools->exit_status = 1;
}

int	handle_home_path(t_tools *tools, char **path)
{
	*path = get_env_value(tools->env_vars, "HOME");
	if (!*path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		tools->exit_status = 1;
		return (1);
	}
	return (0);
}

int	check_cd_arg_count(t_tools *tools, t_simple_cmds *cmd)
{
	if (cmd->str[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		tools->exit_status = 1;
		return (1);
	}
	return (0);
}
