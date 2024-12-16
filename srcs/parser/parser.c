/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/16 14:29:03 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_command(t_lexical **temp, t_simple_cmds **current_cmd,
		t_simple_cmds **cmds_head)
{
	t_simple_cmds	*new_cmd;

	if ((*current_cmd == NULL) || (*temp && ((*temp)->token == TOKEN_PIPE)))
	{
		new_cmd = init_cmd();
		if (!*cmds_head)
			*cmds_head = new_cmd;
		else
		{
			(*current_cmd)->next = new_cmd;
			new_cmd->prev = *current_cmd;
		}
		*current_cmd = new_cmd;
		if ((*temp)->token == TOKEN_PIPE)
			*temp = (*temp)->next;
	}
}

static int	handle_file_errors(t_simple_cmds *current_cmd, t_tools *tools)
{
	if (current_cmd->fd_out == -1 || current_cmd->fd_in == -1)
	{
		ft_putstr_fd("Minishell : ",2);
		ft_putstr_fd(current_cmd->redirections->next->str,2);
		ft_putstr_fd(": No such file or directory\n", 2);
		tools->exit_status = 1;
		free_cmds(&current_cmd);
		return (1);
	}
	return (0);
}

static void	process_command_tokens(t_lexical **temp,
		t_simple_cmds **current_cmd, t_tools *tools)
{
	while (*temp && !check_token(*temp, &tools->heredoc_flag)
		&& (*temp)->token != TOKEN_PIPE)
		handle_words(temp, current_cmd, tools);
}

static int	process_current_command(t_tools *tools, t_lexical **temp,
		t_simple_cmds **current_cmd, t_simple_cmds **cmds_head)
{
	process_command(temp, current_cmd, cmds_head);
	process_command_tokens(temp, current_cmd, tools);
	(*current_cmd)->num_redirections_heredoc = 0;
	while (*temp && check_token(*temp, &tools->heredoc_flag))
	{
		if (handle_redirect_and_check_errors(&tools, temp, current_cmd, *temp))
			return (1);
	}
	check_and_set_redirections(*current_cmd, &tools);
	if (handle_file_errors(*current_cmd, tools))
		return (1);
	process_command_tokens(temp, current_cmd, tools);
	check_and_set_builtin(*current_cmd);
	return (0);
}

t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
	t_simple_cmds	*cmds_head;
	t_simple_cmds	*current_cmd;
	t_lexical		*temp;

	cmds_head = NULL;
	current_cmd = NULL;
	temp = tokens;
	tools->flag = 0;
	tools->heredoc_flag = 0;
	while (temp)
	{
		if (process_current_command(tools, &temp, &current_cmd, &cmds_head))
			return (NULL);
	}
	return (cmds_head);
}
