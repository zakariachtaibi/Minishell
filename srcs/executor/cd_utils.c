/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:11:49 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/09 14:39:54 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	check_cd_arguments(t_tools *tools, t_simple_cmds *cmd)
// {
// 	// int *exit_status = 0;
// 	if (cmd->str[1] == NULL)
// 	{
// 		cmd->str[1] = get_env_value(tools->env_vars, "HOME");
// 		if (cmd->str[1] == NULL)
// 		{
// 			ft_putstr_fd("cd: HOME not set\n", 2);
// 			tools->exit_status = 1;
// 			return (1);
// 		}
// 	}
// 	else if (cmd->str[2] != NULL)
// 	{
// 		fprintf(stderr, "minishell: cd: too many arguments\n");
// 		tools->exit_status = 1;
// 		return (1);
// 	}
// 	else if (cmd->str[2] != NULL)
// 	{
// 		ft_putstr_fd("cd: too many arguments\n", 2);
// 		tools->exit_status = 1;
// 		return (1);
// 	}
// 	return (0);
// }

int check_cd_arguments(t_tools *tools, t_simple_cmds *cmd) {
    if (cmd->str[1] == NULL) {
        cmd->str[1] = get_env_value(tools->env_vars, "HOME");
        if (cmd->str[1] == NULL) {
            ft_putstr_fd("cd: HOME not set\n", 2);
            tools->exit_status = 1;
            return 1; // Early return on error
        }
    } else if (cmd->str[2] != NULL) {
        ft_putstr_fd("cd: too many arguments\n", 2);
        tools->exit_status = 1;
        return 1; // Early return on error
    }
    return 0; // No errors
}

int change_directory(t_tools *tools, t_simple_cmds *cmd) 
{
    // if (strcmp(cmd->str[1], "~") == 0) {
    //     char *home = get_env_value(tools->env_vars, "HOME");
    //     if (home && chdir(home) != 0) {
    //         printf("cd: %s: No such file or directory\n", home);
    //         tools->exit_status = 1;
    //         return 1;
    //     }
     if (chdir(cmd->str[1]) != 0) {
        printf("cd: %s: No such file or directory\n", cmd->str[1]);
        tools->exit_status = 1;
        return 1;
    }
    return 0; // Successful change
}

// int	change_directory(t_tools *tools, t_simple_cmds *cmd)
// {
// 	if (strcmp(cmd->str[1], "~") == 0)
// 		chdir("/nfs/homes/zchtaibi");
// 	else if (chdir(cmd->str[1]) != 0)
// 	{
// 		printf("cd: %s: No such file or directory\n", cmd->str[1]);
// 		tools->exit_status = 1;
// 		return (1);
// 	}
// 	return (0);
// }

t_env_var *find_env_var(t_env_var *env_vars, const char *key) 
{
    while (env_vars) {
        if (strcmp(env_vars->key, key) == 0) {
            return env_vars; // Return directly on match
        }
        env_vars = env_vars->next;
    }
    return NULL; // Not found
}

void update_pwd_variables(t_tools *tools) {
    char *new_pwd = getcwd(NULL, 0);
    if (!new_pwd) {
        perror("getcwd");
        tools->exit_status = 1;
        return;
    }

    t_env_var *pwd = find_env_var(tools->env_vars, "PWD");
    t_env_var *old_pwd = find_env_var(tools->env_vars, "OLDPWD");

    if (pwd) {
        free(old_pwd ? old_pwd->value : NULL);
        if (old_pwd) {
            old_pwd->value = ft_strdup(pwd->value);
        }
        free(pwd->value);
        pwd->value = new_pwd; 
    }
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd) 
{
    if (check_cd_arguments(tools, cmd) || change_directory(tools, cmd)) {
        return 1; 
    }
    update_pwd_variables(tools);
    return 0; 
}
