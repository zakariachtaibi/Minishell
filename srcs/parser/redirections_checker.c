/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:30:41 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/14 12:54:07 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	redir_in(t_simple_cmds **current_cmd, t_lexical **redir)
{
	*redir =(*redir)->next;
	if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
		close((*current_cmd)->fd_in);
	(*current_cmd)->fd_in = open((*redir)->str, O_RDONLY);
	if ((*current_cmd)->fd_in == -1)
		perror("minishell");
}

void	redir_out(t_simple_cmds **current_cmd, t_lexical **redir)
{
	*redir =(*redir)->next;
	if ((*current_cmd)->fd_out != 1 && (*current_cmd)->fd_out != -1)
		close((*current_cmd)->fd_out);
	(*current_cmd)->fd_out = open((*redir)->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*current_cmd)->fd_out == -1)
		perror("minishell");
}

void	redir_heredoc(t_simple_cmds **current_cmd)
{
	char *input;
	int fd;
	fd = open((*current_cmd)->hd_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		return;
	}
	while (1)
	{
		input = readline("> "); 
		if (!input || strcmp(input, (*current_cmd)->hd_file_name) == 0)  
				break;
		write(fd, input, strlen(input));
		write(fd, "\n", 1); 
		free(input);  
	}
	free(input);  
	close(fd);
	fd = open((*current_cmd)->hd_file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: open");
		return;
	}
	(*current_cmd)->fd_in = fd;  
	dup2((*current_cmd)->fd_in, 0);
	close((*current_cmd)->fd_in);
	remove((*current_cmd)->hd_file_name); 
	
}

void redir_append(t_simple_cmds **current_cmd, t_lexical **redir)
{
    *redir = (*redir)->next;
    if ((*current_cmd)->fd_out != 1 && (*current_cmd)->fd_out != -1)
        close((*current_cmd)->fd_out);
    (*current_cmd)->fd_out = open((*redir)->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if ((*current_cmd)->fd_out == -1)
        perror("minishell");
}


void	check_and_set_redirections(t_simple_cmds *current_cmd)
{
	t_lexical	*redir;
	redir = current_cmd->redirections;
	while (redir)
	{
		if (redir->token == TOKEN_REDIRECT_IN)
			redir_in(&current_cmd, &redir);
		else if (redir->token == TOKEN_REDIRECT_OUT)
			redir_out(&current_cmd, &redir);
		else if (redir->token == TOKEN_APPEND)
			redir_append(&current_cmd, &redir);
		else if (redir->token == TOKEN_HEREDOC)
			redir_heredoc(&current_cmd);
		redir = redir->next;
	}
}
