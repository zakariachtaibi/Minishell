/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/21 12:50:28 by hchouai          ###   ########.fr       */
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
    (void) *tools;
    (void) *cmd;
    printf("pwd cmd\n");
     return(1);
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

int builtin_echo(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("echo cmd\n");
    return(1);
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd)
{
    (void) *tools;
    (void) *cmd;
    printf("cd cmd\n");
     return(1);
}