/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/18 13:31:25 by hchouai          ###   ########.fr       */
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
		if (input[i] == '"')
			c++;
		i++;
	}
	if (c % 2 == 0)
		return (1);
	else
		return (0);
}

void	check_token(char *word, t_lexical *node)
{
	if (strcmp(word, "|") == 0)
		node->token = TOKEN_PIPE;
	else if (strcmp(word, "<") == 0)
		node->token = TOKEN_REDIRECT_IN;
	else if (strcmp(word, "<<") == 0)
		node->token = TOKEN_HEREDOC;
	else if (strcmp(word, ">") == 0)
		node->token = TOKEN_REDIRECT_OUT;
	else if (strcmp(word, ">>") == 0)
		node->token = TOKEN_APPEND;
	else
		node->token = TOKEN_WORD;
}

void	check_null(char *str)
{
	if (!str)
		return (NULL);
}

t_lexical	*tokenize(char *input)
{
	char		**word;
	t_lexical	*head;
	t_lexical	*current;
	t_lexical	*node;
	int			i;

	current = NULL;
	head = NULL;
	word = ft_split(input, ' ');
	check_null(word);
	i = 0;
	while (word[i])
	{
		node = malloc(sizeof(t_lexical));
		if (!node)
			exit(1);
		node->str = ft_strdup(word[i]);
		check_null(node->str);
		node->i = i;
		check_token(word[i], node);
		node->next = NULL;
		node->prev = current;
		if (current)
			current->next = node;
		else
			head = node;
		current = node;
		i++;
	}
	free (word);
	return (head);
}
