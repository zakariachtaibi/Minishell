/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/08 18:21:04 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void execute_cmd(t_simple_cmds *current_cmd, t_tools **tools)
// {
    
// }

void execute_commands(t_simple_cmds *cmds_head, t_tools **tools)
{
    // (void) *tools;
    t_simple_cmds *current_cmd = cmds_head;
    while (current_cmd != NULL)
    {
        if (current_cmd->builtin != NULL) 
        {
            current_cmd->builtin(*tools, current_cmd);
        } 
        else
        {
            // execute_cmd(current_cmd, tools);
        }
        current_cmd = current_cmd->next;
    }
}