/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/25 22:29:03 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_lexical		*tokens;
	char			*input;
	t_simple_cmds	*cmds;
	t_tools			*tools;

	if (ac != 1)
	{
		// free_tools(tools);
		// cleanup_readline();
		perror("minishell");
		// printf("wrong number of args");
		exit(1);
	}
	tools = init_tools();
	if (!tools)
	{
		perror("Error initializing tools");
		exit(1);
	}
	tools->std_out = dup(1);
	tools->std_in = dup(0);
	(void)av;
	get_env_vars(tools, envp);
	increment_SHLVL(&tools);
	while (1)
	{
		// signal(SIGINT, handle_sigint);
		// signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		// signal(SIGINT, SIG_IGN);
		if (!input)
		{
			printf("exit\n");
			exit(0);
			break ;
		}
		if (strcmp(input, "") && strcmp(input, "\n"))
			add_history(input);
		input = validat_input(input, tools);
		if (input == NULL)
			continue ;
		tokens = tokenize(input);
		tokens = validate_syntax(tokens, tools);
		if (tokens == NULL)
			continue ;
		cmds = process_tokens(tokens, tools);
		execute_commands(cmds, &tools, tokens);
		dup2(tools->std_out, 1);
		dup2(tools->std_in, 0);
		free(input);
		// ft_free(envp);
		free_lexical(tokens);
		free_cmds(&cmds);
		// system("leaks minishell");
	}
	cleanup_readline();
	free_lexical(tokens);
	free_cmds(&cmds);
	// free_tools(tools);
	return (tools->exit_status);
}
