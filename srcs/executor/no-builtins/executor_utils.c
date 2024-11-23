/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:26:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/23 17:58:18 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*create_env_string(char *key, char *value)
{
	char	*temp_str;
	char	*result;

	temp_str = ft_strjoin(key, "=");
	if (!temp_str)
		return (NULL);
	result = ft_strjoin(temp_str, value);
	free(temp_str);
	return (result);
}

int	fill_envp_array(char **envp, t_env_var *env_vars, int count)
{
	int			i;
	char		*env_str;
	t_env_var	*temp;

	i = 0;
	temp = env_vars;
	while (i < count)
	{
		env_str = create_env_string(temp->key, temp->value);
		if (!env_str)
			return (0);
		envp[i] = env_str;
		temp = temp->next;
		i++;
	}
	envp[count] = NULL;
	return (1);
}

char	**convert_env_vars_to_array(t_env_var *env_vars)
{
	int			count;
	char		**envp;

	count = count_env_vars(env_vars);
	envp = allocate_envp_array(count);
	if (!envp)
		return (NULL);
	if (!fill_envp_array(envp, env_vars, count))
	{
		while (--count >= 0)
			free(envp[count]);
		free(envp);
		return (NULL);
	}
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
