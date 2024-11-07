/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/06 23:46:01 by zchtaibi         ###   ########.fr       */
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
        *tokens = tokenize(input);
        if (*tokens == NULL)
        {
            free(input);
            continue;
        }
        *tokens = validate_syntax((*tokens), (*tools));
        if (*tokens == NULL)
        {
            free_lexical(*tokens); 
            free(input);
            continue;
        }
        *cmds = process_tokens((*tokens), (*tools));
        execute_commands((*cmds), tools, (*tokens));
        dup2((*tools)->std_out, 1);
        dup2((*tools)->std_in, 0);
        free(input);
        free_lexical(*tokens);
        free_cmds(cmds);
    }
}

void free_env_var(t_env_var *env_vars)
{
    t_env_var *tmp;

    while (env_vars)
    {
        tmp = env_vars;
        env_vars = env_vars->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
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
    free_lexical(tokens);
    free_cmds(&cmds);
    free_tools(tools);
    return (e);
}

