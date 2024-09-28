/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/28 19:04:34 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void free_tools(t_tools *tools)
// {
//     if (tools == NULL)
//         return;

//     // Free environment variables
//     t_env_var *current_env = tools->env_vars;
//     t_env_var *next_env;
//     while (current_env)
//     {
//         next_env = current_env->next;
//         free(current_env->key);
//         free(current_env->value);
//         free(current_env);
//         current_env = next_env;
//     }

//     // Free other allocated memory
//     free(tools->var_name);
//     free(tools->var_value);
//     free(tools->working_dir_path);

//     // Close duplicated file descriptors
//     if (tools->std_out > 2)
//         close(tools->std_out);
//     if (tools->std_in > 2)
//         close(tools->std_in);

//     // Finally, free the tools structure itself
//     free(tools);
// }
void free_lexical(t_lexical *head)
{
    t_lexical *current;
    t_lexical *next;

    current = head;
    while (current != NULL)
    {
        next = current->next;
        
        // Free the string if it exists
        if (current->str)
            free(current->str);
        
        // Free the current node
        free(current);
        
        current = next;
    }
}

void free_cmds(t_simple_cmds **cmds)
{
    t_simple_cmds *current;
    t_simple_cmds *next;

    current = *cmds;
    while (current)
    {
        next = current->next;

        // Free the string array
        if (current->str)
        {
            for (int i = 0; current->str[i]; i++)
                free(current->str[i]);
            free(current->str);
        }

        // Free the heredoc file name if it exists
        if (current->hd_file_name)
            free(current->hd_file_name);

        // Free the redirections linked list
        t_lexical *redir = current->redirections;
        while (redir)
        {
            t_lexical *next_redir = redir->next;
            free(redir->str);
            free(redir);
            redir = next_redir;
        }

        // Close file descriptors if they're open
        if (current->fd_in > 2)
            close(current->fd_in);
        if (current->fd_out > 2)
            close(current->fd_out);

        // Free the current command structure
        free(current);

        current = next;
    }
}
t_tools	*init_tools(void)
{
	t_tools	*new_tool;

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

void set_env_var(const char *key, const char *value, t_tools **tools)
{
    t_env_var 	*current;
    t_env_var	*env_var;

	current = (*tools)->env_vars;
	env_var = NULL;
    while (current)
	{
        if (strcmp(current->key, key) == 0)
		{
            env_var = current;
            break;
        }
        current = current->next;
    }
    if (env_var)
	{
        free(env_var->value);
        env_var->value = strdup(value);
    } else
	{
        t_env_var *new_var = (t_env_var *)malloc(sizeof(t_env_var));
        if (!new_var)
            return;
        new_var->key = strdup(key);
        new_var->value = strdup(value);
        new_var->next = (*tools)->env_vars;
        (*tools)->env_vars = new_var;
    }
}

void increment_SHLVL(t_tools **tools)
{
    int shlvl;
    char *shlvl_value;
    char *new_shlvl_value;


    shlvl_value = get_vars_value("SHLVL", *tools);
    if (shlvl_value == NULL)
        shlvl = 0;
    else
        shlvl = ft_atoi(shlvl_value);
    shlvl++;
    new_shlvl_value = ft_itoa(shlvl);
    set_env_var("SHLVL", new_shlvl_value, tools); 
    free(new_shlvl_value);
}


int	main(int ac, char **av, char **envp)
{
    t_lexical		*tokens;
	char			*input;
	t_simple_cmds	*cmds;
	t_tools			*tools;

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
		free_lexical(tokens);
		free_cmds(&cmds);
		system("leaks minishell");
	}
	return (tools->exit_status);
}
