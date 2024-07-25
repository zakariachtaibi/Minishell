/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/25 11:57:46 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_sorted_env(t_tools *tools) {
    // Create a copy of the environment list
    t_env_var *copy = NULL, *temp = tools->env_vars;
    while (temp) {
        t_env_var *new_node = malloc(sizeof(t_env_var));
        new_node->key = ft_strdup(temp->key);
        new_node->value = ft_strdup(temp->value);
        new_node->next = copy;
        copy = new_node;
        temp = temp->next;
    }

    // Sort the copied list using while loop
    t_env_var *i = copy;
    while (i) {
        t_env_var *j = i->next;
        while (j) {
            if (ft_strncmp(i->key, j->key, ft_strlen(i->key) + 1) > 0) {
                // Swap key and value
                char *temp_key = i->key;
                char *temp_value = i->value;
                i->key = j->key;
                i->value = j->value;
                j->key = temp_key;
                j->value = temp_value;
            }
            j = j->next;
        }
        i = i->next;
    }

    // Print the sorted list
    t_env_var *current = copy;
    while (current) {
        printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }

    // Free the copied list
    while (copy) {
        t_env_var *next = copy->next;
        free(copy->key);
        free(copy->value);
        free(copy);
        copy = next;
    }
}

int builtin_export(t_tools *tools, t_simple_cmds *cmd) {
    int i = 1;
    if (cmd->str[i] == NULL) {
        print_sorted_env(tools);
        return 0;
    }

    while (cmd->str[i]) {
        char *equal_sign = ft_strchr(cmd->str[i], '=');
        if (equal_sign) {
            char *key = ft_substr(cmd->str[i], 0, equal_sign - cmd->str[i]);
            char *value = ft_strdup(equal_sign + 1);
            if (!key || !value) {
                // Handle allocation failure
                free(key);
                free(value);
                return 1;
            }

            t_env_var *current = tools->env_vars;
            t_env_var *prev = NULL;
            while (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) < 0)
            {
                prev = current;
                current = current->next;
            }

            if (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0) {
        
                free(current->value);
                current->value = value;
                free(key);
            } else {
                t_env_var *new_var = malloc(sizeof(t_env_var));
                // if (!new_var) {
                //     free(key);
                //     free(value);
                //     return 1;
                // }
                new_var->key = key;
                new_var->value = value;
                new_var->next = current;

                if (prev) {
                    prev->next = new_var;
                } else {
                    tools->env_vars = new_var;
                }
            }
        } else {
            printf("export: `%s`: not a valid identifier\n", cmd->str[i]);
        }
        i++;
    }
    return 0;
}

int builtin_pwd(t_tools *tools, t_simple_cmds *cmd)
{
    (void)cmd;
    (void)tools;
    char *buff;
    buff= malloc(1024 * sizeof(char));
    if (!buff)
    {
        perror("malloc");
        return (1);
    }
    if (getcwd(buff, 1024) == NULL)
    {
        perror("getcwd");
        free(buff);
        return (1);
    }
    printf("%s\n", buff);
    tools->working_dir_path = buff;
    free(buff);
    return (0); 
}

int builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
    (void)tools;

    int exit_status = 0;

    if (cmd->str[1] != NULL)
        exit_status = ft_atoi(cmd->str[0]);
    exit(exit_status);
}

int builtin_unset(t_tools *tools, t_simple_cmds *cmd)
{
    t_env_var *current = tools->env_vars;
    int i = 1;
    
    while(cmd->str[i])
    {
        // printf("%s", cmd->str[i]);
        current =  tools->env_vars;
        while(current)
        {
            if(!(strcmp(current->key,cmd->str[i])))
            {
                delete_node_env(&(tools->env_vars), current);
               break ;
            }
           current = current->next; 
        }
        i++;
    }
    return(1);
}

int builtin_env(t_tools *tools, t_simple_cmds *cmd)
{
    int i;

    i = 1;
    if (cmd->str[i] == NULL)
    {
        t_env_var *current = tools->env_vars;
        while (current)
        {
            printf("%s=\"%s\"\n", current->key, current->value);
            current = current->next;
        }
        return (0);
    }
    return (1);
}

int	builtin_echo(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
	int j = 1;
	while(cmd->str[j])
	{
		ft_putstr_fd(cmd->str[j],1);
		ft_putchar_fd(' ',1);
		j++;
	}
	printf("\n");
    return (0);
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd)
{
    (void)tools;
    if (cmd->str[1] == NULL)
    {
        write(2, "cd: missing argument\n", 22);
        return (1);        
    }
    if(chdir(cmd->str[1]) != 0)
    {
        printf("cd: %s: No such file or directory\n", cmd->str[1]);
        return (1);
    }
    return (0);
}