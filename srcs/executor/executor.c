/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/21 11:10:22 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_cmd(t_simple_cmds *current_cmd, t_tools **tools)
{
    if (execute_if_absolute_path(current_cmd, tools))
        return;

    char **split = get_path_dirs((*tools)->env_vars);
    if (split && execute_from_path(split, current_cmd, tools))
        return;

    handle_command_not_found(current_cmd, tools);
}

void execute_commands(t_simple_cmds *cmds_head, t_tools **tools)
{
    t_simple_cmds *current_cmd;

    current_cmd = cmds_head;
    while (current_cmd != NULL)
    {
        if (current_cmd->builtin != NULL) 
            current_cmd->builtin(*tools, current_cmd);
        else
            execute_cmd(current_cmd, tools);
        current_cmd = current_cmd->next;
    }
}

