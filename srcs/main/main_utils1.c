/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 23:10:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 23:12:56 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	tokenize_and_execute(t_tools **tools, t_lexical **tokens,
		t_simple_cmds **cmds, char *input)
{
	*tokens = tokenize(input);
	if (*tokens == NULL)
		return (0);
	*tokens = validate_syntax(*tokens, *tools);
	if (*tokens == NULL)
		return (0);
	*cmds = process_tokens(*tokens, *tools);
	execute_commands(*cmds, tools, *tokens);
	return (1);
}

void	process_shell_input(t_tools **tools, t_lexical **tokens,
		t_simple_cmds **cmds, char *input)
{
	if (ft_strcmp(input, "") && ft_strcmp(input, "\n"))
		add_history(input);
	input = validat_input(input, *tools);
	if (!input || !*input)
	{
		free(input);
		return ;
	}
	if (!tokenize_and_execute(tools, tokens, cmds, input))
	{
		free(input);
		return ;
	}
	dup2((*tools)->std_out, 1);
	dup2((*tools)->std_in, 0);
	free(input);
	free_cmds(cmds);
	if (*tokens)
		free_lexical(*tokens);
}
