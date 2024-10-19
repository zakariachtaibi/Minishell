/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:42:32 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/19 13:24:17 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sort_env_vars(t_env_var *copy)
{
	t_env_var	*i;
	t_env_var	*j;

	i = copy;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0)
			{
				swap_values(&(i->key), &(i->value), &(j->key), &(j->value));
			}
			j = j->next;
		}
		i = i->next;
	}
}

void	print_env_vars(t_env_var *copy)
{
	t_env_var	*current;

	current = copy;
	while (current)
	{
		check_env_vars(&current);
		current = current->next;
	}
}

void	free_env_vars(t_env_var *copy)
{
	t_env_var	*next;

	while (copy)
	{
		next = copy->next;
		free(copy->key);
		free(copy->value);
		free(copy);
		copy = next;
	}
}

char	*get_env_value(t_env_var *env_vars, const char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env_vars)
	{
		if (!ft_strncmp(env_vars->key, key, key_len)
			&& env_vars->key[key_len] == '\0')
			return (env_vars->value);
		env_vars = env_vars->next;
	}
	return (NULL);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
