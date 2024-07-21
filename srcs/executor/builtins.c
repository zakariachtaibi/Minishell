/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/21 18:10:53 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("export cmd\n");
     return(1);
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
    free(buff);
    return (0); 
}

int builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("exit cmd\n");
     return(1);
}

int builtin_unset(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("unset cmd\n");
     return(1);
}

int builtin_env(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("env cmd\n");
     return(1);
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
    (void) *tools;
    (void) *cmd;
    printf("cd cmd\n");
     return(1);
}