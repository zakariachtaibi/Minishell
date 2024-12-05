/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:30:41 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/04 17:32:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir_in(t_simple_cmds **current_cmd, t_lexical **redir)
{
	*redir = (*redir)->next;
	if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
		close((*current_cmd)->fd_in);
	(*current_cmd)->fd_in = open((*redir)->str, O_RDONLY);
}

void	redir_out(t_simple_cmds **current_cmd, t_lexical **redir)
{
	*redir = (*redir)->next;
	if ((*current_cmd)->fd_out != 1 && (*current_cmd)->fd_out != -1)
		close((*current_cmd)->fd_out);
	if ((*current_cmd)->fd_in != -1)
		(*current_cmd)->fd_out = open((*redir)->str, O_WRONLY | O_CREAT
				| O_TRUNC,
				0644);
}

void	redir_append(t_simple_cmds **current_cmd, t_lexical **redir)
{
	*redir = (*redir)->next;
	if ((*current_cmd)->fd_out != 1 && (*current_cmd)->fd_out != -1)
		close((*current_cmd)->fd_out);
	(*current_cmd)->fd_out = open((*redir)->str, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if ((*current_cmd)->fd_out == -1)
		perror("minishell");
}

void	check_and_set_redirections(t_simple_cmds *current_cmd, t_tools **tools)
{
	t_lexical	*redir;

	(void)tools;
	redir = current_cmd->redirections;
	while (redir)
	{
		if (redir->token == TOKEN_REDIRECT_IN)
			redir_in(&current_cmd, &redir);
		else if (redir->token == TOKEN_REDIRECT_OUT)
			redir_out(&current_cmd, &redir);
		else if (redir->token == TOKEN_APPEND)
			redir_append(&current_cmd, &redir);
		// else if (redir->token == TOKEN_HEREDOC)
		// 	redir_heredoc(&current_cmd, &redir, (*tools));
		redir = redir->next;
	}
}
