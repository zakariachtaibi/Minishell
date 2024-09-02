/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/31 22:34:05 by zchtaibi         ###   ########.fr       */
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
	if (!ft_strncmp(str, "|", 2))
		node->token = TOKEN_PIPE;
	else if (!ft_strncmp(str, "<", 2))
		node->token = TOKEN_REDIRECT_IN;
	else if (!ft_strncmp(str, "<<", 2))
		node->token = TOKEN_HEREDOC;
	else if (!ft_strncmp(str, ">", 2))
		node->token = TOKEN_REDIRECT_OUT;
	else if (!ft_strncmp(str, ">>", 2))
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
	char		token[1024];

	head 	= NULL;
	current = NULL;
	i		= 0;
	j 		= 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		j = 0;
		while (input[i] && (input[i] != ' ' || (input[i] == ' ' && input[i - 1] == '\\')))
		{
			if (input[i] == '\\')
				i++;
			token[j++] = input[i++];
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





