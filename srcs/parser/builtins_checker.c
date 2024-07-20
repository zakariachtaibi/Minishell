/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:04:13 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 21:20:22 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_builtin check_builtins_type(char *str)
{
    if(strcmp(str, "echo") == 0)
        return(BUILTIN_ECHO);
    if(strcmp(str, "cd") == 0)
        return(BUILTIN_CD);
    if(strcmp(str, "pwd") == 0)
        return(BUILTIN_PWD);
    if(strcmp(str, "export") == 0)
        return(BUILTIN_EXPORT);
    if(strcmp(str, "unset") == 0)
        return(BUILTIN_UNSET);
    if(strcmp(str, " env") == 0)
        return(BUILTIN_ENV);
     if(strcmp(str, "exit") == 0)
        return(BUILTIN_EXIT);    
    else
        return(BUILTIN_NONE);
}

int (*get_builtin_func(t_builtin type))(t_tools *, t_simple_cmds *)
{
    if (type == BUILTIN_ECHO)
		return (builtin_echo);
	else if (type == BUILTIN_CD)
		return (builtin_cd);
    else if (type == BUILTIN_PWD)
        return (builtin_pwd);
    else if (type == BUILTIN_EXPORT)
        return (builtin_export);
    else if (type == BUILTIN_UNSET)
        return (builtin_unset);
    else if (type == BUILTIN_ENV)
        return (builtin_env);
	else if (type == BUILTIN_EXIT)
        return (builtin_exit);
    return (NULL);
}


void check_and_set_builtin(t_simple_cmds *cmd)
{
    if (cmd->str && cmd->str[0])
	{
        t_builtin type = check_builtins_type(cmd->str[0]);
        if (type != BUILTIN_NONE) 
		{
            cmd->builtin = get_builtin_func(type);
            
        } else 
		{
            cmd->builtin = NULL;
        }
    } else 
	{
        cmd->builtin = NULL;
    }
}
