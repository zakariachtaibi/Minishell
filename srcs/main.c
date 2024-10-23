/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/23 16:12:37 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_lexical		*tokens;
	char			*input;
	t_simple_cmds	*cmds;
	t_tools			*tools;
	// char	*ttname;
	// ttname=ttyname(1);
	// ttname=ft_substr(ttname, 9, ft_strlen(ttname));
	// printf("%s\n", ttname);

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
			tools->exit_status = 130;
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
