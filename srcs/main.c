/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/06 09:37:35 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_lexical		*tokens;
	char			*input;
	t_simple_cmds	*cmds;
	t_tools			*tools;
	int				std_out;
	int				std_in;
	std_out = dup(1);
	std_in = dup(0);
	(void)av;
	if (ac != 1)
	{
		printf("wrong number of args");
		exit(1);
	}
		
	tools = malloc(sizeof(t_tools));
	get_env_vars(tools, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			exit(1);
		if(strcmp(input,"") && strcmp(input, "/n"))
			add_history(input);
		input = validat_input(input);
		if (input == NULL) 
			continue ;
		tokens = tokenize(input);
		tokens = validate_syntax(tokens);
		if (tokens == NULL)
			continue ;
		cmds = process_tokens(tokens, tools);
		execute_commands(cmds, &tools);
		dup2(std_out, 1);
		dup2(std_in, 0);
		free(input);
	}
}
