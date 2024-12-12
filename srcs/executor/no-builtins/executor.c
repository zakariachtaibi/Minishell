/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 13:26:26 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/12 15:29:39 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	handle_last_builtin(t_exec *ctx)
{
	int	old_stdout;
	int	old_stdin;

	if (ctx->current_cmd->builtin != NULL && ctx->current_cmd->next == NULL
		&& ctx->prev_pipe_read == STDIN_FILENO)
	{
		old_stdout = dup(STDOUT_FILENO);
		old_stdin = dup(STDIN_FILENO);
		if (ctx->current_cmd->fd_in != STDIN_FILENO)
			dup2(ctx->current_cmd->fd_in, STDIN_FILENO);
		if (ctx->current_cmd->fd_out != STDOUT_FILENO)
			dup2(ctx->current_cmd->fd_out, STDOUT_FILENO);
		(*ctx->tools)->exit_status = ctx->current_cmd->builtin(*ctx->tools,
				ctx->current_cmd, ctx->tokens);
		ctx->builtin_executed = 1;
		dup2(old_stdout, STDOUT_FILENO);
		dup2(old_stdin, STDIN_FILENO);
		close(old_stdout);
		close(old_stdin);
		return (1);
	}
	return (0);
}

static void	redirect_io(t_exec *ctx)
{
	if (ctx->prev_pipe_read != STDIN_FILENO)
	{
		dup2(ctx->prev_pipe_read, STDIN_FILENO);
		close(ctx->prev_pipe_read);
	}
	if (ctx->current_cmd->next)
	{
		close(ctx->pipe_fd[0]);
		dup2(ctx->pipe_fd[1], STDOUT_FILENO);
		close(ctx->pipe_fd[1]);
	}
	if (ctx->current_cmd->fd_in != STDIN_FILENO)
	{
		dup2(ctx->current_cmd->fd_in, STDIN_FILENO);
		close(ctx->current_cmd->fd_in);
	}
	if (ctx->current_cmd->fd_out != STDOUT_FILENO)
	{
		dup2(ctx->current_cmd->fd_out, STDOUT_FILENO);
		close(ctx->current_cmd->fd_out);
	}
}

static void	execute_child_process(t_exec *ctx)
{
	setup_child_signals();
	if (ctx->current_cmd->redirections)
	{
		if (ctx->current_cmd->redirections->token == TOKEN_HEREDOC)
			redir_heredoc(&ctx->current_cmd, &ctx->current_cmd->redirections,
				*ctx->tools);
	}
	redirect_io(ctx);
	if (ctx->current_cmd->builtin != NULL)
	{
		ctx->e = ctx->current_cmd->builtin(*ctx->tools, ctx->current_cmd,
				ctx->tokens);
	}
	else
	{
		execute_cmd(ctx->current_cmd, ctx->tools, ctx->tokens);
		ctx->e = (*ctx->tools)->exit_status;
	}
	free_cmds(&ctx->cmds_head);
	free_tools(*ctx->tools);
	free_lexical(ctx->tokens);
	exit(ctx->e);
}

static void	handle_command_execution(t_exec *ctx)
{
	if (ctx->current_cmd->next && pipe(ctx->pipe_fd) == -1)
		exit(EXIT_FAILURE);
	if (handle_last_builtin(ctx))
	{
		ctx->current_cmd = ctx->current_cmd->next;
		return ;
	}
	ctx->pid = fork();
	if (ctx->pid == 0)
		execute_child_process(ctx);
	else if (ctx->pid < 0)
	{
		free_cmds(&(ctx->cmds_head));
		free_env_var((*ctx->tools)->env_vars);
		exit(EXIT_FAILURE);
	}
	else
		parent_pipe_handle(ctx);
	ctx->current_cmd = ctx->current_cmd->next;
}

void	execute_commands(t_simple_cmds *cmds_head, t_tools **tools,
		t_lexical *tokens)
{
	t_exec	ctx;

	init_execution_context(&ctx, cmds_head, tools, tokens);
	while (ctx.current_cmd)
		handle_command_execution(&ctx);
	wait_processes(&ctx);
}
