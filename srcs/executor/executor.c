/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/21 13:39:15 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_commands(t_simple_cmds *cmds_head, t_tools *tools)
{
    (void) *tools;
    t_simple_cmds *current_cmd = cmds_head;
    while (current_cmd != NULL) {
        if (current_cmd->builtin != NULL) {
            current_cmd->builtin(tools, current_cmd);
        } else {
            // Code to execute non-builtin commands goes here
        }
        current_cmd = current_cmd->next;
    }
}