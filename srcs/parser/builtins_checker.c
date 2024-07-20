/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 19:04:13 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 20:05:28 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_builtin check_builtins_type(char *str)
{
    int i = 0;
    if(ft_strcmp(str, "echo") == 0)
        return(BUILTIN_ECHO);
    if(ft_strcmp(str, "cd") == 0)
        return(BUILTIN_CD);
    if(ft_strcmp(str, "pwd") == 0)
        return(BUILTIN_PWD);
    if(ft_strcmp(str, "export") == 0)
        return(BUILTIN_EXPORT);
    if(ft_strcmp(str, "unset") == 0)
        return(BUILTIN_UNSET);
    if(ft_strcmp(str, " env") == 0)
        return(BUILTIN_ENV);
     if(ft_strcmp(str, "exit") == 0)
        return(BUILTIN_EXIT);    
    else
        return(BUILTIN_NONE);
}

int (*get_builtin_func(t_builtin type))(t_tools *, t_simple_cmds *)
{
    if(type == 0)
        return (builtin_echo);
     if(type == 1)
        return (builtin_cd);
     if(type == 2)
        return (builtin_pwd);
     if(type == 3)
        return (builtin_export);
     if(type == 4)
        return (builtin_unset);
     if(type == 5)
        return (builtin_env);
     if(type == 6)
        return (builtin_exit);
    return (NULL);
}