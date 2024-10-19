/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:59:43 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/19 19:21:07 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_plus_equal(t_tools *tools, char *str, char **key, char **value)
{
	char	*plus_equal_sign;
	char	*existing_value;
	char	*temp;

	plus_equal_sign = ft_strnstr(str, "+=", ft_strlen(str));
	*key = ft_substr(str, 0, plus_equal_sign - str);
	*value = ft_strdup(plus_equal_sign + 2);
	existing_value = get_env_value(tools->env_vars, *key);
	if (existing_value)
	{
		temp = ft_strjoin(existing_value, *value);
		free(*value);
		*value = temp;
	}
}

void	handle_equal(char *str, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(str, '=');
	*key = ft_substr(str, 0, equal_sign - str);
	if (*(equal_sign + 1) == '\0')
		*value = ft_strdup(" ");
	else
		*value = ft_strdup(equal_sign + 1);
}

void	process_export(t_tools *tools, char **str, int *i)
{
	char	*key;
	char	*value;
	char	*plus_equal_sign;
	char	*equal_sign;
	int		in;
	// t_tools *tmp = tools;

	plus_equal_sign = ft_strnstr(str[*i], "+=", ft_strlen(str[*i]));
	equal_sign = ft_strchr(str[*i], '=');
	in = 0;
	if (str[*i][0] == '\0')
	{
		printf("minishell: export: `%s': not a valid identifier\n", str[*i]);
		tools->exit_status = 1;
		return ;
	}
	if (plus_equal_sign)
		handle_plus_equal(tools, str[*i], &key, &value);
	else if (equal_sign)
	{
		handle_equal(str[*i], &key, &value);
	}
		
	else
	{
		key = ft_strdup(str[*i]);
		value = NULL;
	}
	while (key[in] != '\0')
	{
		if (!(ft_isalpha(key[in]) || (in > 0 && ft_isalnum(key[in])))
			|| key[in] == 32)
		{
			printf("minishell: export: `%s': not a valid identifier\n",
					str[*i]);
			free(key);
			free(value);
			tools->exit_status = 1;
			return ;
		}
		in++;
	}
	if (key[0] != '\0')
		handle_env_var(&tools, key, value);
	else
	{
		printf("minishell: export: `%s': not a valid identifier\n", str[*i]);
		free(key);
		if (value)
			free(value);
		tools->exit_status = 1;
	}
}

void	print_sorted_env(t_tools *tools)
{
	t_env_var	*copy;

	copy = copy_env_vars(tools->env_vars);
	sort_env_vars(copy);
	print_env_vars(copy);
	free_env_vars(copy);
}

int	builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
	int i = 1;
	if (!cmd->str[i])
	{
		print_sorted_env(tools);
		tools->exit_status = 0;
		return (0);
	}
	while (cmd->str[i])
	{
		process_export(tools, cmd->str, &i);
		i++;
	}
	return (tools->exit_status);
}