/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:41:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 22:29:13 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	is_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '"')
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

char	*unescape_spaces(char *str, int flag)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	if (str[0] == '\0')
		return (ft_strdup("\0"));
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ' && flag == 1)
		{
			free(result);
			return (NULL);
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	free_lexical_node(t_lexical *node)
{
	if (node)
	{
		free(node->str);
		free(node);
	}
}

int	ft_strlen_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}
