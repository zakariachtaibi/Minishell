/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/21 18:30:10 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tools	*init_tools(void)
{
	t_tools	*new_tool ;

	new_tool = malloc(sizeof(t_tools));
	new_tool->env_vars = NULL;
	new_tool->var_start = 0;
	new_tool->var_len = 0;
	new_tool->var_name = NULL;
	new_tool->var_value = NULL;
	new_tool->working_dir_path = NULL;
	new_tool->exit_status = 0;
	new_tool->std_out = 0;
	new_tool->std_in = 0;
	return (new_tool);
}

int main(int ac, char **av, char **envp)
{
    t_lexical       *tokens;
    char            *input;
    t_simple_cmds   *cmds;
    t_tools         *tools;
    
    tools = init_tools();
    tools->std_out = dup(1);
    tools->std_in = dup(0);
    (void)av;
    if (ac != 1)
    {
        printf("wrong number of args");
        exit(1);
    }
    get_env_vars(tools, envp);
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
            break;
        }
        if(strcmp(input,"") && strcmp(input, "\n"))
            add_history(input);
        input = validat_input(input , tools);
        if (input == NULL)
            continue;
        tokens = tokenize(input);
        tokens = validate_syntax(tokens, tools);
        if (tokens == NULL)
            continue;
        cmds = process_tokens(tokens, tools);
        execute_commands(cmds, &tools, tokens);
        dup2(tools->std_out, 1);
        dup2(tools->std_in, 0);
        free(input);
    }
    return tools->exit_status;
}
