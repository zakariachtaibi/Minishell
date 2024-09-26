/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:30:41 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/24 17:29:07 by hchouai          ###   ########.fr       */
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
	(*current_cmd)->fd_out = open((*redir)->str, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
}

void	redir_heredoc(t_simple_cmds **current_cmd, t_lexical **redir,
		t_tools *tools)
{
	char	*input;
	int		fd;
	int		f;

	f = 0;
	*redir = (*redir)->next;
	if ((*current_cmd)->num_redirections_heredoc > 16)
	{
		printf("minishell: maximum here-document count exceeded\n");
		exit(2);
	}
	(*current_cmd)->hd_file_name = ft_strdup((*redir)->str);
	if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
		close((*current_cmd)->fd_in);
	fd = open((*current_cmd)->hd_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return ;
	}
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (strcmp(input, (*redir)->str) == 0)
		{
			free(input);
			break ;
		}
		if ((*redir)->filename_flag == 2)
			input = expand_vars(tools, input, &f, 0);
		write(fd, input, strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	(*current_cmd)->fd_in = open((*current_cmd)->hd_file_name, O_RDONLY);
	if ((*current_cmd)->fd_in == -1)
		perror("minishell");
	if (unlink((*current_cmd)->hd_file_name) == -1)
		perror("minishell: failed to remove heredoc temp file");
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
		else if (redir->token == TOKEN_HEREDOC)
			redir_heredoc(&current_cmd, &redir, (*tools));
		redir = redir->next;
	}
}
