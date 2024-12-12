/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_checker_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:53:02 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/12 14:50:54 by zchtaibi         ###   ########.fr       */
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
	else if (*i + 1 < len && (input[*i + 1] == '?' || ft_isalnum(input[*i
					+ 1]) || input[*i + 1] == '_'))
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

char	*expand_inside_heredoc(t_tools *tools, char *input)
{
	char	*expanded_word;
	size_t	i;
	char	*new_expansion;
	char	*temp_word;
	size_t	len;

	i = 0;
	len = ft_strlen(input);
	expanded_word = ft_strdup("");
	while (i < len)
	{
		new_expansion = NULL;
		if (input[i] == '$')
			new_expansion = expand_var(input, &i, tools, len);
		else
			new_expansion = expand_plain_text(input, &i);
		if (new_expansion && *new_expansion)
		{
			temp_word = ft_strjoin(expanded_word, new_expansion);
			free(expanded_word);
			free(new_expansion);
			expanded_word = temp_word;
		}
	}
	return (expanded_word);
}

void	heredoc_loop(t_simple_cmds **current_cmd, t_lexical **redir, t_tools *tools, int fd)
{
	char	*input;
	char	*expanded;
	(void)current_cmd;

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

void	redir_heredoc(t_simple_cmds **current_cmd, t_lexical **redir,
		t_tools *tools)
{
	int		fd;
	char	*ttname;
	char	*temp;
	char	*temp2;
	pid_t	pid_c;
	int		status;

	ttname = ttyname(1);
	temp = ft_substr(ttname, 9, ft_strlen(ttname));
	temp2 = ft_strjoin("/tmp/", temp);
	free(temp);
	ttname = temp2;
	*redir = (*redir)->next;
	if ((*current_cmd)->num_redirections_heredoc > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		free(ttname);
		exit(2);
	}
	pid_c = fork();
	if (pid_c == 0)
	{
		fd = open(ttname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("minishell");
			free(ttname);
			return ;
		}
		heredoc_loop(current_cmd, redir, tools, fd);
		free(ttname);
		// free_cmds(current_cmd);
		// free_tools(tools);
		close(fd);
		exit(0);
	}
	else if (pid_c < 0)
	{
		perror("fork fail!");
		return ;
	}
	else
	{
		waitpid(pid_c, &status, 0);
		if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
			close((*current_cmd)->fd_in);
		if ((*current_cmd)->fd_in != -1)
			(*current_cmd)->fd_in = open(ttname, O_RDONLY);
		if (unlink(ttname) == -1)
			perror("minishell: failed to remove heredoc temp file");
		free(ttname);
	}
}

