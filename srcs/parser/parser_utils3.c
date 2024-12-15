/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:21:30 by mac               #+#    #+#             */
/*   Updated: 2024/12/15 22:13:15 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		int *i, t_tools *tools)
{
	char	**tmp;
	int		in;

	in = 0;
	tmp = ft_split(expanded, ' ');
	while (tmp[in])
	{
		reallocate_str_array(current_cmd, i, &tools->word_count,
			tools->count_str);
		(*current_cmd)->str[(*i)++] = ft_strdup(tmp[in]);
		in++;
	}
	free(expanded);
	ft_free(tmp);
}

void	handle_expansion(char *expanded, t_simple_cmds **current_cmd,
	int *i, t_tools *tools)
{
	reallocate_str_array(current_cmd, i, &tools->word_count, tools->count_str);
	(*current_cmd)->str[(*i)++] = expanded;
}

void	process_word(t_lexical **temp, t_tools *tools,
		t_simple_cmds **current_cmd, int *i)
{
	char	*expanded;

	expanded = expand_vars(tools, (*temp)->str);
	if (expanded && tools->flag == 1 && is_space(expanded) == 1)
		split_expanded_words(expanded, current_cmd, i, tools);
	else if (expanded)
		handle_expansion(expanded, current_cmd, i, tools);
	*temp = (*temp)->next;
}

void	handle_words(t_lexical **temp, t_simple_cmds **current_cmd,
		t_tools *tools)
{
	char	**new_str;
	int		i;

	count_words(temp, &tools->word_count);
	initialize_str_array(current_cmd, tools->word_count,
		&tools->count_str, &new_str);
	i = tools->count_str;
	while (*temp && (*temp)->token == TOKEN_WORD)
	{
		if (ft_strcmp((*temp)->str, "export") == 0)
			tools->export_flag = 1;
		process_word(temp, tools, current_cmd, &i);
	}
	(*current_cmd)->str[i] = NULL;
}
