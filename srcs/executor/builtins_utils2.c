/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 13:11:55 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/04 18:20:25 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap_values(char **ikey, char **ivalue, char **jkey, char **jvalue)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = ft_strdup(*ikey);
	if ((*ivalue) == NULL)
		temp_value = NULL;
	else
		temp_value = ft_strdup(*ivalue);
	free(*ikey);
	free(*ivalue);
	(*ikey) = ft_strdup(*jkey);
	if ((*jvalue) == NULL)
		(*ivalue) = NULL;
	else
		(*ivalue) = ft_strdup(*jvalue);
	free(*jkey);
	free(*jvalue);
	(*jkey) = temp_key;
	(*jvalue) = temp_value;
}

void	check_env_vars(t_env_var **current)
{
	if ((*current)->value == NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd((*current)->key, 1);
		ft_putstr_fd("\n", 1);
	}
	else
	{
		if ((*current)->value[0] == '\0')
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd((*current)->key, 1);
			ft_putstr_fd("=\"\"", 1);
			ft_putstr_fd("\n", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd((*current)->key, 1);
			ft_putstr_fd("=", 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd((*current)->value, 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd("\n", 1);
		}
	}
}

int	is_valid_identifier(const char *str)
{
	if (!str || !*str || *str == '{' || (*str >= '0' && *str <= '9'))
		return (0);
	while (*str)
	{
		if (!(*str == '_' || (*str >= 'a' && *str <= 'z') || (*str >= 'A'
					&& *str <= 'Z') || (*str >= '0' && *str <= '9')))
			return (0);
		str++;
	}
	return (1);
}
