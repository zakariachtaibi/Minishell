/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/10 21:24:05 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void minishell_loop(t_tools **tools, t_lexical **tokens,
				t_simple_cmds **cmds)
{
    char *input;

    while (1)
    {
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);
        input = readline("minishell> ");
        signal(SIGINT, SIG_IGN);
        if (!input)
        {
            printf("exit\n");
            free_tools(*tools);
			exit(0);
            break;
        }
        if (ft_strcmp(input, "") && ft_strcmp(input, "\n"))
            add_history(input);
        input = validat_input(input, (*tools));
        if (input == NULL)
        {
            free(input); 
            continue;
        }
        // input = remove_consecutive_quotes(input);
        if(!*input)
            input=ft_strdup("");
        *tokens = tokenize(input);
        if (*tokens == NULL)
        {
        
            free(input);
            continue;
        }
        *tokens = validate_syntax((*tokens), (*tools));
        if (*tokens == NULL)
        {
            free(input);
            continue;
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
}

int main(int ac, char **av, char **envp)
{
    t_tools         *tools;
    t_lexical       *tokens = NULL;
    t_simple_cmds   *cmds = NULL;
    int             e;
    (void)av;

    if (ac != 1)
    {
        perror("minishell");
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
    get_env_vars(tools, envp);
    increment_SHLVL(&tools);
    minishell_loop(&tools, &tokens, &cmds);
    e = tools->exit_status;
    cleanup_readline();
    free_cmds(&cmds);
    free_tools(tools);
    return (e);
}

