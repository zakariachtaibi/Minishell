/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 18:49:59 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 19:03:52 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtin_echo(t_tools *tools, t_simple_cmds *cmd)
{
    printf("echo cmd\n");
}

int builtin_cd(t_tools *tools, t_simple_cmds *cmd)
{
    printf("cd cmd\n");
}

int builtin_pwd(t_tools *tools, t_simple_cmds *cmd)
{
    printf("pwd cmd\n");
}

int builtin_export(t_tools *tools, t_simple_cmds *cmd)
{
    printf("export cmd\n");
}

int builtin_unset(t_tools *tools, t_simple_cmds *cmd)
{
    printf("unset cmd\n");
}

int builtin_env(t_tools *tools, t_simple_cmds *cmd)
{
    printf("env cmd\n");
}

int builtin_exit(t_tools *tools, t_simple_cmds *cmd)
{
    printf("exit cmd\n");
}