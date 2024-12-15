/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:53:02 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 23:13:22 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*create_temp_filename(void)
{
	char	*ttname;
	char	*temp;
	char	*temp2;

	ttname = ttyname(1);
	temp = ft_substr(ttname, 9, ft_strlen(ttname));
	temp2 = ft_strjoin("/tmp/", temp);
	free(temp);
	return (temp2);
}

static void	write_heredoc_input(int fd, t_tools *tools, char *input,
		int filename_flag)
{
	char	*expanded;

	expanded = input;
	if (filename_flag == 2)
	{
		expanded = expand_inside_heredoc(tools, input);
		free(input);
	}
	if (!expanded)
		expanded = ft_strdup("");
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static int	process_heredoc_input(int fd, t_lexical *redir, t_tools *tools)
{
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, redir->str) == 0)
		{
			free(input);
			break ;
		}
		write_heredoc_input(fd, tools, input, redir->filename_flag);
	}
	return (0);
}

static int	finalize_heredoc(t_simple_cmds *cmd, char *filename)
{
	cmd->fd_in = open(filename, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		perror("minishell");
		return (0);
	}
	if (unlink(filename) == -1)
		perror("minishell: temp file remove failed");
	return (1);
}

void	redir_heredoc(t_simple_cmds **cmd, t_lexical **redir, t_tools *tools)
{
	char	*ttname;
	int		fd;

	if ((*cmd)->num_redirections_heredoc > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		exit (2);
	}
	ttname = create_temp_filename();
	*redir = (*redir)->next;
	if ((*cmd)->fd_in > 0)
		close((*cmd)->fd_in);
	fd = open(ttname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		free(ttname);
		return ;
	}
	process_heredoc_input(fd, *redir, tools);
	close(fd);
	finalize_heredoc(*cmd, ttname);
	free(ttname);
}
