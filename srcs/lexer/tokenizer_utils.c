/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 20:00:58 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/23 20:18:50 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_lexical	*create_token_node(char *token, int i)
{
	t_lexical	*node;

	node = malloc(sizeof(t_lexical));
	if (!node)
		return (NULL);
	node->str = ft_strdup(token);
	if (!node->str)
	{
		free(node);
		return (NULL);
	}
	node->i = i;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

static void	set_token_type(t_lexical *node)
{
	if (!ft_strcmp(node->str, "|"))
		node->token = TOKEN_PIPE;
	else if (!ft_strcmp(node->str, "<"))
		node->token = TOKEN_REDIRECT_IN;
	else if (!ft_strcmp(node->str, "<<"))
		node->token = TOKEN_HEREDOC;
	else if (!ft_strcmp(node->str, ">"))
		node->token = TOKEN_REDIRECT_OUT;
	else if (!ft_strcmp(node->str, ">>"))
		node->token = TOKEN_APPEND;
	else
		node->token = TOKEN_WORD;
}

static t_lexical	*check_node(char *str, int i)
{
	t_lexical	*node;

	node = create_token_node(str, i);
	if (!node)
		return (NULL);
	set_token_type(node);
	return (node);
}

t_lexical	*add_node_to_list(t_lexical **head, t_lexical **current,
		char *token, int i)
{
	t_lexical	*node;

	node = check_node(token, i);
	if (!node)
	{
		free_lexical(*head);
		return (NULL);
	}
	if (*current)
	{
		(*current)->next = node;
		node->prev = *current;
	}
	else
		*head = node;
	*current = node;
	return (node);
}
