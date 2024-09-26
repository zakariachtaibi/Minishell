/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:26:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/16 18:02:45 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_env_vars(t_env_var *env_vars)
{
	int			count;
	t_env_var	*temp;

	count = 0;
	temp = env_vars;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

char	**allocate_envp_array(int count)
{
	char	**envp;

	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	return (envp);
}

void	fill_envp_array(char **envp, t_env_var *env_vars, int count)
{
	t_env_var	*temp;
	int			i;
	char		*temp_value;

	temp = env_vars;
	i = 0;
	while (i < count)
	{
		envp[i] = ft_strjoin(temp->key, "=");
		temp_value = envp[i];
		envp[i] = ft_strjoin(envp[i], temp->value);
		free(temp_value);
		temp = temp->next;
		i++;
	}
	envp[count] = NULL;
}

char	**convert_env_vars_to_array(t_env_var *env_vars)
{
	int		count;
	char	**envp;

	count = count_env_vars(env_vars);
	envp = allocate_envp_array(count);
	if (!envp)
		return (NULL);
	fill_envp_array(envp, env_vars, count);
	return (envp);
}

char	**get_path_dirs(t_env_var *env_vars)
{
	while (env_vars)
	{
		if (!ft_strncmp(env_vars->key, "PATH", 5))
			return (ft_split(env_vars->value, ':'));
		env_vars = env_vars->next;
	}
	return (NULL);
}

void	handle_command_not_found(t_simple_cmds *current_cmd, t_tools **tools)
{
	ft_putstr_fd(current_cmd->str[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	(*tools)->exit_status = 127;
}