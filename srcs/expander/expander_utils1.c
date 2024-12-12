/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:09:34 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/12 15:09:10 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_single_quote(const char *current_word, size_t *j, int *flag)
{
	*flag = 0;
	return (expand_single_quote(current_word, j));
}

char	*handle_double_quote(t_tools *tools, const char *current_word,
		size_t *j)
{
	tools->flag = 0;
	(*j)++;
	return (expand_double_quote(tools, current_word, j));
}

char	*handle_dollar_sign(t_tools *tools, const char *current_word, size_t *j,
		size_t len)
{
	if (tools->heredoc_flag == 1)
		return (NULL);
	if (*j + 1 < len && ft_isdigit(current_word[*j + 1]))
	{
		(*j) += 2;
		return (NULL);
	}
	else if (*j + 1 < len && current_word[*j + 1] == '$')
	{
		(*j) += 2;
		tools->flag = 1;
		return (ft_itoa(getpid()));
	}
	else if (*j + 1 < len && (current_word[*j + 1] == '?'
				|| ft_isalnum(current_word[*j + 1]) || current_word[*j
				+ 1] == '_'))
	{
		tools->flag = 1;
		return (expand_variable(tools, current_word, j));
	}
	else
	{
		(*j)++;
		tools->flag = 0;
		return (*j == len) ? ft_strdup("$") : ft_strdup("");
	}
}

char	*handle_plain_text(const char *current_word, size_t *j, int *flag)
{
	*flag = 2;
	return (expand_plain_text(current_word, j));
}

void	append_expansion(char **expanded_word, char *new_expansion)
{
	char	*temp_word;

	temp_word = ft_strjoin(*expanded_word, new_expansion);
	free(*expanded_word);
	free(new_expansion);
	*expanded_word = temp_word;
}

char	*expand_vars(t_tools *tools, char *current_word)
{
	size_t	j;
	size_t	len;
	char	*expanded_word;
	char	*new_expansion;

	j = 0;
	len = ft_strlen(current_word);
	expanded_word = NULL;
	while (j < len)
	{
		new_expansion = NULL;
		if (current_word[j] == '\'')
		{
			new_expansion = handle_single_quote(current_word, &j, &tools->flag);
		}
		else if (current_word[j] == '"')
		{
			new_expansion = handle_double_quote(tools, current_word, &j);
		}
		else if (current_word[j] == '$')
		{
			new_expansion = handle_dollar_sign(tools, current_word, &j, len);
		}
		else
		{
			new_expansion = handle_plain_text(current_word, &j, &tools->flag);
		}
		if (new_expansion)
		{
			append_expansion(&expanded_word, new_expansion);
		}
	}
	return (expanded_word);
}

// char	*expand_vars(t_tools *tools, char *current_word, int *flag,
// 		int heredoc_flag)
// {
// 	size_t	j;
// 	char	*expanded_word;
// 	size_t	len;
// 	char	*new_expansion;
// 	char	*temp_word;

// 	j = 0;
// 	expanded_word = NULL;
// 	len = ft_strlen(current_word);
// 	while (j < len)
// 	{
// 		new_expansion = NULL;
// 		if (current_word[j] == '\'')
// 		{
// 			new_expansion = expand_single_quote(current_word, &j);
// 			*flag = 0;
// 		}
// 		else if (current_word[j] == '"')
// 		{
// 			j++;
// 			new_expansion = expand_double_quote(tools, current_word, &j,
// 					heredoc_flag);
// 			*flag = 0;
// 		}
// 		else if (current_word[j] == '$' && heredoc_flag == 0)
// 		{
// 			if (j + 1 < len && ft_isdigit(current_word[j + 1]))
// 				j += 2;
// 			else if (j + 1 < len && current_word[j + 1] == '$')
// 			{
// 				new_expansion = ft_itoa(getpid());
// 				j += 2;
// 				*flag = 1;
// 			}
// 			else if (j + 1 < len && (current_word[j + 1] == '?'
// 					|| ft_isalnum(current_word[j + 1]) || current_word[j
// 						+ 1] == '_'))
// 			{
// 				new_expansion = expand_variable(tools, current_word, &j);
// 				*flag = 1;
// 			}
// 			else
// 			{
// 				if (current_word[j + 1] == '\0')
// 					new_expansion = ft_strdup("$");
// 				else
// 					new_expansion = ft_strdup("");
// 				j++;
// 				*flag = 0;
// 			}
// 		}
// 		else
// 		{
// 			new_expansion = expand_plain_text(current_word, &j);
// 			*flag = 2;
// 		}
// 		if (new_expansion)
// 		{
// 			temp_word = ft_strjoin(expanded_word, new_expansion);
// 			free(expanded_word);
// 			free(new_expansion);
// 			expanded_word = temp_word;
// 		}
// 	}
// 	return (expanded_word);
// }