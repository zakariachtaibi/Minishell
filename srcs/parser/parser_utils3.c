/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:21:30 by mac               #+#    #+#             */
/*   Updated: 2024/12/13 14:40:51 by mac              ###   ########.fr       */
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
	for (int i = 0; i < *count_str; i++)
		(*new_str)[i] = (*current_cmd)->str[i];
	free((*current_cmd)->str);
	(*current_cmd)->str = *new_str;
}

void	reallocate_str_array(t_simple_cmds **current_cmd, int *i,
		int *word_count, int count_str)
{
	int	new_size;

	if (*i >= (*word_count + count_str))
	{
		new_size = (*word_count + count_str) * 2;
		(*current_cmd)->str = realloc((*current_cmd)->str, sizeof(char *)
				* (new_size + 1));
		if (!(*current_cmd)->str)
		{
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		*word_count = new_size - count_str;
	}
}

void	split_expanded_words(char *expanded, t_simple_cmds **current_cmd,
		int *i, int *word_count, int count_str)
{
	char	**tmp;
	int		in;

	in = 0;
	tmp = ft_split(expanded, ' ');
	while (tmp[in])
	{
		reallocate_str_array(current_cmd, i, word_count, count_str);
		(*current_cmd)->str[(*i)++] = ft_strdup(tmp[in]);
		in++;
	}
	free(expanded);
	ft_free(tmp);
}

void	handle_expansion(char *expanded, t_simple_cmds **current_cmd, int *i,
		int *word_count, int count_str)
{
	reallocate_str_array(current_cmd, i, word_count, count_str);
	(*current_cmd)->str[(*i)++] = expanded;
}

void	process_word(t_lexical **temp, t_tools *tools,
		t_simple_cmds **current_cmd, int *i, int *word_count, int count_str)
{
	char	*expanded;

	expanded = expand_vars(tools, (*temp)->str);
	if (expanded && tools->flag == 1 && is_space(expanded) == 1)
		split_expanded_words(expanded, current_cmd, i, word_count, count_str);
	else if (expanded)
		handle_expansion(expanded, current_cmd, i, word_count, count_str);
	*temp = (*temp)->next;
}

void	handle_words(t_lexical **temp, t_simple_cmds **current_cmd,
		t_tools *tools)
{
	int		word_count;
	int		count_str;
	char	**new_str;
	int		i;

	count_words(temp, &word_count);
	initialize_str_array(current_cmd, word_count, &count_str, &new_str);
	i = count_str;
	while (*temp && (*temp)->token == TOKEN_WORD)
	{
		if (ft_strcmp((*temp)->str, "export") == 0)
			tools->export_flag = 1;
		process_word(temp, tools, current_cmd, &i, &word_count, count_str);
	}
	(*current_cmd)->str[i] = NULL;
}
