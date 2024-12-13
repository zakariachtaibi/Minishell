/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:53:02 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 13:57:07 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_var(char *input, size_t *i, t_tools *tools, size_t len)
{
	char	*new_expansion;

	new_expansion = NULL;
	if (*i + 1 < len && ft_isdigit(input[*i + 1]))
		*i += 2;
	else if (*i + 1 < len && input[*i + 1] == '$')
	{
		new_expansion = ft_itoa(getpid());
		*i += 2;
	}
	else if (*i + 1 < len && (input[*i + 1] == '?' || ft_isalnum(input[*i + 1])
			|| input[*i + 1] == '_'))
		new_expansion = expand_variable(tools, input, i);
	else
	{
		if (input[*i + 1] == '\0')
			new_expansion = ft_strdup("$");
		else
			new_expansion = ft_strdup("");
		*i += 1;
	}
	return (new_expansion);
}

void	heredoc_loop(t_lexical **redir, t_tools *tools, int fd)
{
	char	*input;
	char	*expanded;

	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strcmp(input, (*redir)->str) == 0)
		{
			free(input);
			break ;
		}
		if ((*redir)->filename_flag == 2)
		{
			expanded = expand_inside_heredoc(tools, input);
			if (!expanded)
				expanded = ft_strdup("");
			free(input);
			input = expanded;
		}
		write(fd, input, strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
}

void	handle_heredoc_child(t_lexical **redir, t_tools *tools, char *ttname)
{
	int	fd;

	fd = open(ttname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		free(ttname);
		exit(1);
	}
	heredoc_loop(redir, tools, fd);
	close(fd);
	free(ttname);
	exit(0);
}

void	manage_heredoc_parent(t_simple_cmds **current_cmd, char *ttname,
		int status)
{
	waitpid(-1, &status, 0);
	if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
		close((*current_cmd)->fd_in);
	if ((*current_cmd)->fd_in != -1)
		(*current_cmd)->fd_in = open(ttname, O_RDONLY);
	if (unlink(ttname) == -1)
		perror("minishell: failed to remove heredoc temp file");
	free(ttname);
}

void	redir_heredoc(t_simple_cmds **current_cmd, t_lexical **redir,
		t_tools *tools)
{
	char	*ttname;
	pid_t	pid_c;

	ttname = generate_temporary_filename();
	*redir = (*redir)->next;
	if ((*current_cmd)->num_redirections_heredoc > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		free(ttname);
		exit(2);
	}
	pid_c = fork();
	if (pid_c == 0)
		handle_heredoc_child(redir, tools, ttname);
	else if (pid_c < 0)
	{
		perror("fork fail!");
		free(ttname);
		return ;
	}
	manage_heredoc_parent(current_cmd, ttname, 0);
}
