/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:06:17 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 22:28:47 by hchouai          ###   ########.fr       */
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

int	handle_redirect_and_check_errors(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, t_lexical *tokens)
{
	(*tools)->flag = handle_redirections(tools, temp, current_cmd, tokens);
	if ((*tools)->flag == 1)
	{
		ft_putstr_fd((*temp)->str, 2);
		ft_putstr_fd(" : ambiguous redirect\n", 2);
		(*tools)->exit_status = 1;
		free_cmds(current_cmd);
		return (1);
	}
	return (0);
}
