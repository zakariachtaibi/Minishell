/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:06:17 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 23:14:23 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	count_words(t_lexical **temp, int *word_count)
{
	t_lexical	*word_temp;

	*word_count = 0;
	word_temp = *temp;
	while (word_temp && word_temp->token == TOKEN_WORD)
	{
		(*word_count)++;
		word_temp = word_temp->next;
	}
}

void	initialize_str_array(t_simple_cmds **current_cmd, int word_count,
		int *count_str, char ***new_str)
{
	int	i;

	if ((*current_cmd)->str)
		*count_str = ft_strlen_array((*current_cmd)->str);
	else
		*count_str = 0;
	*new_str = malloc(sizeof(char *) * (word_count + *count_str + 1));
	if (!*new_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < *count_str)
	{
		(*new_str)[i] = (*current_cmd)->str[i];
		i++;
	}
	free((*current_cmd)->str);
	(*current_cmd)->str = *new_str;
}
