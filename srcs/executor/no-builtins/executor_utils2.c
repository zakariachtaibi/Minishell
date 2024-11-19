/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 22:05:15 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/20 00:13:56 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	wait_processes(t_exec *ctx)
{
	if (!ctx->builtin_executed)
	{
		waitpid(ctx->last_pid, &ctx->status, 0);
		if (WIFEXITED(ctx->status))
		{
			ctx->child_exit_status = WEXITSTATUS(ctx->status);
			if (ctx->child_exit_status != 0)
				(*ctx->tools)->exit_status = ctx->child_exit_status;
		}
	}
	while (wait(NULL) > 0)
		continue ;
}

void	parent_pipe_handle(t_exec *ctx)
{
	if (ctx->prev_pipe_read != STDIN_FILENO)
		close(ctx->prev_pipe_read);
	if (ctx->current_cmd->next)
	{
		close(ctx->pipe_fd[1]);
		ctx->prev_pipe_read = ctx->pipe_fd[0];
	}
	ctx->last_pid = ctx->pid;
}

void init_execution_context(t_exec *ctx, t_simple_cmds *cmds_head,
		t_tools **tools, t_lexical *tokens)
{
	ctx->cmds_head = cmds_head;
	ctx->tools = tools;
	ctx->tokens = tokens;
	ctx->prev_pipe_read = STDIN_FILENO;
	ctx->current_cmd = cmds_head;
}