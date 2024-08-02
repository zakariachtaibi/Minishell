/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:04:13 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/02 11:10:26 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_builtin	check_builtins_type(char *str)
{
	if (!ft_strncmp(str, "echo", 5))
		return (BUILTIN_ECHO);
	if (!ft_strncmp(str, "cd", 3))
		return (BUILTIN_CD);
	if (!ft_strncmp(str, "pwd", 4) || !ft_strncmp(str, "(pwd)", 6))
		return (BUILTIN_PWD);
	if (!ft_strncmp(str, "export", 7))
		return (BUILTIN_EXPORT);
	if (!ft_strncmp(str, "unset", 6))
		return (BUILTIN_UNSET);
	if (!ft_strncmp(str, "env", 4))
		return (BUILTIN_ENV);
	if (!ft_strncmp(str, "exit", 5))
		return (BUILTIN_EXIT);
	else
		return (BUILTIN_NONE);
}

int	(*get_builtin_func(t_builtin type))(t_tools *tools, t_simple_cmds *cmds)
{
	if (type == BUILTIN_ECHO)
		return (builtin_echo);
	else if (type == BUILTIN_CD)
		return (builtin_cd);
	else if (type == BUILTIN_PWD)
		return (builtin_pwd);
	else if (type == BUILTIN_EXPORT)
		return (builtin_export);
	else if (type == BUILTIN_UNSET)
		return (builtin_unset);
	else if (type == BUILTIN_ENV)
		return (builtin_env);
	else if (type == BUILTIN_EXIT)
		return (builtin_exit);
	return (NULL);
}

void	check_and_set_builtin(t_simple_cmds *cmd)
{
	t_builtin	type;

	if (cmd->str && cmd->str[0])
	{
		type = check_builtins_type(cmd->str[0]);
		if (type != BUILTIN_NONE)
			cmd->builtin = get_builtin_func(type);
		else
			cmd->builtin = NULL;
	}
	else
		cmd->builtin = NULL;
}
