/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/19 15:23:41 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_lexical	*check_node(char *str, int i)
{
	t_lexical	*node;
	int k;

	node = malloc(sizeof(t_lexical));
	k = 0;
	if (!node)
		exit(1);
	if (has_unclosed_double_quotes(str))
	{
		printf("Error: Unclosed quotes\n");
		free(str);
	}
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
		return (NULL);
	while (word[i])
	{
		node = check_node(word[i], i);
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
