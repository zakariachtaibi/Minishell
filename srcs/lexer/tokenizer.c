/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/18 14:58:49 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_unclosed_quotes(char *temp)
{
	char	*input;
	int		i;
	int		c;

	input = temp;
	i = 0;
	c = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'' )
			c++;
		i++;
	}
	if (c % 2 == 0)
		return (1);
	else
		return (0);
}

t_lexical	*handel_node(char *str, int i)
{
	t_lexical	*node;

	node = malloc(sizeof(t_lexical));
	if (!node || !(node->str = ft_strdup(str)))
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
	node->next = node->prev = NULL;
	return node;
}

t_lexical *tokenize(char *input)
{
	char		**word;
	t_lexical	*head;
	t_lexical	*current;
	t_lexical	*node;
	int			i;

	word = ft_split(input, ' ');
	head = NULL;
	current = NULL;
	i = 0;
	if (!word)
		return NULL;
	while (word[i])
	{
		node = handel_node(word[i], i);
		node->prev = current;
		if (current)
			current->next = node;
		else
			head = node;
		current = node;
		i++;
	}
	free(word);
	return (head);
}
