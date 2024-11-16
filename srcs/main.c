/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/16 02:38:44 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int sig_flag = 0;

void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        sig_flag = 1;
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void minishell_loop(t_tools **tools, t_lexical **tokens,
				t_simple_cmds **cmds)
{
    char    *input;
    int     e;

    while (1)
    {
        signal(SIGINT, handle_sigint);
        signal(SIGQUIT, SIG_IGN);
        input = readline("minishell> ");
        signal(SIGINT, SIG_IGN);
        if (sig_flag == 1)
            (*tools)->exit_status = 130;
        if (!input)
        {
            printf("exit\n");
            e = (*tools)->exit_status;
            free_tools(*tools);
			exit(e);
            break;
        }
        sig_flag = 0;
        if (ft_strcmp(input, "") && ft_strcmp(input, "\n"))
            add_history(input);
        input = validat_input(input, (*tools));
        if (input == NULL)
        {
            free(input); 
            continue;
        }
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

