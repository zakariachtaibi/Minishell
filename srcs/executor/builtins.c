/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/09 13:56:16 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_tools *tools, t_simple_cmds *cmd)
{

	(void)cmd;
	  char *buff;
	buff = malloc(1024 * sizeof(char));
	if (!buff)
	{
		perror("malloc");
		// tools->exit_status = 1;
		return (1);
	}
	if (getcwd(buff, 1024) == NULL)
	{
		perror("getcwd");
		free(buff);
		// tools->exit_status = 1;
		return (1);
	}
	ft_putstr_fd(buff, 1);
	printf("\n");
	tools->working_dir_path = ft_strdup(buff);
	free(buff);
	return (0);
} //leaks fixed;

int	builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
	int	exit_status;

	if (cmd->str[1] == NULL)
		exit(tools->exit_status);
	if (!is_numeric(cmd->str[1]))
	{
		ft_putstr_fd("exit\n", 2);
		printf("minishell: exit: %s: numeric argument required\n", cmd->str[1]);
		exit(2);
	}
	if (cmd->str[2] != NULL)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_status = atoi(cmd->str[1]);
	exit(exit_status);
}

int builtin_unset(t_tools *tools, t_simple_cmds *cmd) {
    int i = 1;
    while (cmd->str[i]) {
        t_env_var *current = tools->env_vars;
        // t_env_var *prev = NULL;
        while (current) 
		{
            if (strcmp(current->key, cmd->str[i]) == 0) {
                // Delete the node
                delete_node_env(&(tools->env_vars), current);
                break; 
            }
            // prev = current;
            current = current->next;
        }
        if (!current) {
            fprintf(stderr, "minishell: unset: %s: no such variable\n", cmd->str[i]);
        }
        i++;
    }
    return 0;
}//leaks fixed

int builtin_env(t_tools *tools, t_simple_cmds *cmd) {
    int i = 1;
    t_env_var *current = tools->env_vars;

    if (cmd->str[i] == NULL) {
        if (!current) {
            printf("No environment variables set.\n");
            return 0;
        }
        while (current) {
            if (current->value) { // Only print if the value is non-NULL
                printf("%s=%s\n", current->key, current->value);
            }
            current = current->next;
        }
        return 0;
    }
    return 1; // Indicate invalid usage
}// Leaks fixed

int	builtin_echo(t_tools *tools, t_simple_cmds *cmd)
{
	int	j;
	
	int	flag;

	(void)*tools;
	j = 1;
	flag = 0;
	search_for_argn(cmd, &flag, &j);
	while (cmd->str[j])
	{
		ft_putstr_fd(cmd->str[j], 1);
		if (cmd->str[j + 1] != NULL)
			ft_putchar_fd(' ', 1);
		j++;
	}
	if (!flag)
		ft_putchar_fd('\n', 1);
	return (0);
}// leaks fixed
