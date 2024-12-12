/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/11 17:54:46 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_sig_flag = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		g_sig_flag = 1;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	process_shell_input(t_tools **tools, t_lexical **tokens,
		t_simple_cmds **cmds, char *input)
{
	if (ft_strcmp(input, "") && ft_strcmp(input, "\n"))
		add_history(input);
	input = validat_input(input, (*tools));
	if (input == NULL)
		return ;
	if (!*input)
		input = ft_strdup("");
	*tokens = tokenize(input);
	if (tokens == NULL)
	{
		free(input);
		return ;
	}
	*tokens = validate_syntax((*tokens), (*tools));
	if (*tokens == NULL)
	{
		free(input);
		return ;
	}
	*cmds = process_tokens((*tokens), (*tools));
	execute_commands((*cmds), tools, (*tokens));
	dup2((*tools)->std_out, 1);
	dup2((*tools)->std_in, 0);
	free(input);
	free_cmds(cmds);
	if (*tokens != NULL)
		free_lexical(*tokens);
}

void	handle_shell_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	minishell_loop(t_tools **tools, t_lexical **tokens,
		t_simple_cmds **cmds)
{
	char	*input;
	int		e;

	while (1)
	{
		handle_shell_signals();
		input = readline("minishell> ");
		signal(SIGINT, SIG_IGN);
		if (g_sig_flag == 1)
			(*tools)->exit_status = 130;
		if (!input)
		{
			ft_putstr_fd("exit\n", 1);
			e = (*tools)->exit_status;
			free_tools(*tools);
			exit(e);
		}
		g_sig_flag = 0;
		process_shell_input(tools, tokens, cmds, input);
	}
}

int	initialize_shell(t_tools **tools, char **envp)
{
	*tools = init_tools();
	if (!*tools)
	{
		perror("Error initializing tools");
		exit(1);
	}
	(*tools)->std_out = dup(1);
	(*tools)->std_in = dup(0);
	get_env_vars(*tools, envp);
	increment_shlvl(tools);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_tools			*tools;
	t_lexical		*tokens;
	t_simple_cmds	*cmds;
	int				e;

	tokens = NULL;
	cmds = NULL;
	(void)av;
	if (ac != 1)
	{
		perror("minishell");
		exit(1);
	}
	initialize_shell(&tools, envp);
	minishell_loop(&tools, &tokens, &cmds);
	e = tools->exit_status;
	cleanup_readline();
	free_cmds(&cmds);
	free_tools(tools);
	return (e);
}
