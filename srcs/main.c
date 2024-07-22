/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/22 12:25:19 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_lexical		*tokens;
	char			*input;
	t_simple_cmds	*cmds;
	t_tools			*tools;

	(void)av;
	if (ac != 1)
		exit(1);
	tools = malloc(sizeof(t_tools));
	init_env_vars(tools, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			exit(1);
		add_history(input);
		input = validat_input(input);
		if (input == NULL)
			continue ;
		tokens = tokenize(input);
		tokens = validate_syntax(tokens);
		if (tokens == NULL)
			continue ;
		cmds = process_tokens(tokens);
		execute_commands(cmds, &tools);
		free(input);
	}
}
