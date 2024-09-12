/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/12 10:22:41 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lexical	*check_node(char *str, int i)
{
	t_lexical	*node;

	node = malloc(sizeof(t_lexical));
	if (!node)
		exit(1);
	node->str = ft_strdup(str);
	if (!node->str)
		exit(1);
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
	j = 0;
	in_quotes = 0;
	quote_char = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		j = 0;
		while (input[i] && (in_quotes || (input[i] != ' ' && input[i] != '\t')))
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
				token[j++] = input[i];
			}
			else
				token[j++] = input[i];
			i++;
		}
		token[j] = '\0';
		if (j > 0)
		{
			node = check_node(token, i);
			node->prev = current;
			if (current)
				current->next = node;
			else
				head = node;
			current = node;
		}
	}
	return (head);
}






