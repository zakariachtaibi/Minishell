/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:10:15 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/23 20:18:28 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_operator_token(t_tokenizer *tok, char *input)
{
	if (tok->j > 0)
	{
		tok->token[tok->j] = '\0';
		if (!add_node_to_list(&tok->head, &tok->current, tok->token, tok->i))
			return (0);
		tok->j = 0;
	}
	tok->token[tok->j++] = input[tok->i++];
	if ((tok->token[0] == '>' && input[tok->i] == '>')
		|| (tok->token[0] == '<' && input[tok->i] == '<'))
		tok->token[tok->j++] = input[tok->i++];
	return (1);
}

static int	process_single_token(t_tokenizer *tok, char *input)
{
	char	quote_char;

	if (input[tok->i] == '"' || input[tok->i] == '\'')
	{
		quote_char = input[tok->i];
		tok->token[tok->j++] = input[tok->i++];
		while (input[tok->i] && input[tok->i] != quote_char)
			tok->token[tok->j++] = input[tok->i++];
		if (input[tok->i])
			tok->token[tok->j++] = input[tok->i++];
	}
	else if (input[tok->i] == '>' || input[tok->i] == '<'
		|| input[tok->i] == '|')
		return (0);
	else
		tok->token[tok->j++] = input[tok->i++];
	return (1);
}

static int	process_token(t_tokenizer *tok, char *input)
{
	tok->j = 0;
	while (input[tok->i] && input[tok->i] != ' ' && input[tok->i] != '\t')
	{
		if (!process_single_token(tok, input))
		{
			if (!handle_operator_token(tok, input))
				return (0);
			break ;
		}
	}
	if (tok->j > 0)
	{
		tok->token[tok->j] = '\0';
		if (!add_node_to_list(&tok->head, &tok->current, tok->token, tok->i))
			return (0);
	}
	return (1);
}

t_lexical	*tokenize(char *input)
{
	t_tokenizer	tok;

	tok.head = NULL;
	tok.current = NULL;
	tok.i = 0;
	while (input[tok.i])
	{
		while (input[tok.i] == ' ' || input[tok.i] == '\t')
			tok.i++;
		if (!input[tok.i])
			break ;
		if (!process_token(&tok, input))
			return (NULL);
	}
	return (tok.head);
}
