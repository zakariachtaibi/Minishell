/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:51:51 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/14 22:46:17 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_exit_cleanup(t_exec *ctx)
{
	free_cmds(&ctx->cmds_head);
	free_tools(*ctx->tools);
	free_lexical(ctx->tokens);
}

t_tools	*init_tools(void)
{
	t_tools	*new_tool;

	new_tool = malloc(sizeof(t_tools));
	if (!new_tool)
		return (NULL);
	new_tool->env_vars = NULL;
	new_tool->var_start = 0;
	new_tool->var_len = 0;
	new_tool->var_name = NULL;
	new_tool->var_value = NULL;
	new_tool->working_dir_path = NULL;
	new_tool->exit_status = 0;
	new_tool->std_out = 0;
	new_tool->std_in = 0;
	return (new_tool);
}

void	set_env_var(const char *key, const char *value, t_tools **tools)
{
	t_env_var	*current;
	t_env_var	*env_var;
	t_env_var	*new_var;

	current = (*tools)->env_vars;
	env_var = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			env_var = current;
			break ;
		}
		current = current->next;
	}
	if (env_var)
	{
		free(env_var->value);
		env_var->value = ft_strdup(value);
	}
	else
	{
		new_var = (t_env_var *)malloc(sizeof(t_env_var));
		if (!new_var)
			return ;
		new_var->key = ft_strdup(key);
		new_var->value = ft_strdup(value);
		new_var->next = (*tools)->env_vars;
		(*tools)->env_vars = new_var;
	}
}

void	free_str_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	increment_shlvl(t_tools **tools)
{
	int		shlvl;
	char	*shlvl_value;
	char	*new_shlvl_value;

	shlvl_value = get_vars_value("SHLVL", *tools);
	if (shlvl_value == NULL)
		shlvl = 0;
	else
		shlvl = ft_atoi(shlvl_value);
	shlvl++;
	new_shlvl_value = ft_itoa(shlvl);
	set_env_var("SHLVL", new_shlvl_value, tools);
	free(new_shlvl_value);
}
