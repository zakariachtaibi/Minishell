/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:00:42 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/21 18:56:44 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*get_current_working_directory(t_tools *tools)
{
	char	*buff;
	char	*pwd_value;

	buff = malloc(1024 * sizeof(char));
	if (!buff)
	{
		perror("malloc");
		return (NULL);
	}
	if (getcwd(buff, 1024) == NULL)
	{
		pwd_value = get_env_value(tools->env_vars, "PWD");
		if (pwd_value)
		{
			free(buff);
			return (ft_strdup(pwd_value));
		}
		perror("getcwd");
		free(buff);
		return (NULL);
	}
	return (buff);
}

void	perform_exit_cleanup(t_simple_cmds *cmd, t_lexical *tokens,
		t_tools *tools)
{
	cleanup_readline();
	if (tokens)
		free_lexical(tokens);
	free_cmds(&cmd);
	free_tools(tools);
}

int	validate_unset_identifier(char *identifier)
{
	if (!is_valid_identifier(identifier))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(identifier, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (0);
	}
	return (1);
}

int	remove_env_variable(t_tools *tools, char *key)
{
	t_env_var	*current;
	t_env_var	*node_to_delete;

	current = tools->env_vars;
	node_to_delete = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
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
			return (1);
	}
	return (0);
}

int	env_error(t_tools *tools, char *arg)
{
	ft_putstr_fd("env: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	tools->exit_status = 1;
	return (1);
}
