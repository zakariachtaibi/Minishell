/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/04 16:38:11 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lexical	*check_node(char *str, int i)
{
	t_lexical	*node;

	node = malloc(sizeof(t_lexical));
	if (!node)
		return (NULL);
	node->str = ft_strdup(str);
	if (!node->str)
	{
		free(node);
		return (NULL);
	}
	node->i = i;
	if (!strcmp(str, "|"))
		node->token = TOKEN_PIPE;
	else if (!strcmp(str, "<"))
		node->token = TOKEN_REDIRECT_IN;
	else if (!strcmp(str, "<<"))
		node->token = TOKEN_HEREDOC;
	else if (!strcmp(str, ">"))
		node->token = TOKEN_REDIRECT_OUT;
	else if (!strcmp(str, ">>"))
		node->token = TOKEN_APPEND;
	else
		node->token = TOKEN_WORD;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_lexical	*tokenize(char *input)
{
	t_lexical	*head;
	t_lexical	*current;
	t_lexical	*node;
	int			i;
	int			j;
	int			in_quotes;
	char		token[1024];
	char		quote_char;

	head = NULL;
	current = NULL;
	i = 0;
	in_quotes = 0;
	quote_char = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		j = 0;
		while (input[i])
		{
			if (input[i] == '"' || input[i] == '\'')
			{
				if (!in_quotes)
				{
					in_quotes = 1;
					quote_char = input[i];
				}
				else if (in_quotes && input[i] == quote_char)
					in_quotes = 0;
				token[j++] = input[i++];
			}
			else if (!in_quotes && (input[i] == '>' || input[i] == '<'
					|| input[i] == '|'))
			{
				if (j > 0)
				{
					token[j] = '\0';
					node = check_node(token, i);
					if (!node)
					{
						free_lexical(head);
						return (NULL);
					}
					if (current)
					{
						current->next = node;
						node->prev = current;
					}
					else
						head = node;
					current = node;
					j = 0;
				}
				token[j++] = input[i++];
				if ((token[0] == '>' && input[i] == '>') || (token[0] == '<'
						&& input[i] == '<'))
					token[j++] = input[i++];
				break ;
			}
			else if (!in_quotes && (input[i] == ' ' || input[i] == '\t'))
				break ;
			else
				token[j++] = input[i++];
		}
		if (j > 0)
		{
			token[j] = '\0';
			node = check_node(token, i);
			if (!node)
			{
				free_lexical(head);
				return (NULL);
			}
			if (current)
			{
				current->next = node;
				node->prev = current;
			}
			else
				head = node;
			current = node;
		}
	}
	return (head);
}
