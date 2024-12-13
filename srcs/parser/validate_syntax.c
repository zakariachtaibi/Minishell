/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 17:36:11 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 13:57:46 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_invalid_redirections(t_lexical **tokens)
{
	t_lexical	*current;

	current = *tokens;
	while ((current))
	{
		if (current->token != 5 && current->token != 0)
		{
			if ((current->next == NULL) || ((current->next)->token != 5))
				return (1);
		}
		else if (current->token == 0)
		{
			if ((current->next == NULL) || ((current->next)->token == 0)
				|| (current->prev == NULL))
				return (10);
		}
		current = current->next;
	}
	return (0);
}

static void	print_syntax_error(char *token, int is_newline)
{
	ft_putstr_fd("syntax error near unexpected token '", 2);
	if (is_newline)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}

static int	handle_syntax_error(t_lexical *tokens, int error_code,
		t_tools *tools)
{
	if (error_code == 10)
		print_syntax_error(tokens->str, 0);
	else if (!tokens->next->next)
		print_syntax_error(NULL, 1);
	else
		print_syntax_error(tokens->next->str, 0);
	tools->exit_status = 2;
	free_lexical(tokens);
	return (0);
}

t_lexical	*validate_syntax(t_lexical *tokens, t_tools *tools)
{
	t_lexical	*temp;
	int			error_code;

	temp = tokens;
	error_code = has_invalid_redirections(&temp);
	if (error_code)
	{
		handle_syntax_error(tokens, error_code, tools);
		return (NULL);
	}
	return (tokens);
}

char	*generate_temporary_filename(void)
{
	char	*temp;
	char	*temp2;

	temp = ft_substr(ttyname(1), 9, ft_strlen(ttyname(1)));
	temp2 = ft_strjoin("/tmp/", temp);
	free(temp);
	return (temp2);
}
