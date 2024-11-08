/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:30:41 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/08 01:55:00 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_inside_heredoc(t_tools *tools, char *input)
{
	char *expanded_word;
	size_t i = 0;
	char *new_expansion;
	char *temp_word;
	size_t len = ft_strlen(input);
	expanded_word = ft_strdup("");
	
	while(i < len)
	{
		new_expansion = NULL;
		if(input[i] == '$')
		{
			if(i + 1 < len && ft_isdigit(input[i + 1]))
				i += 2;
			else if (i + 1 < len && input[i + 1] == '$')
			{
				new_expansion = ft_itoa(getpid());
				i += 2;
			}
			else if (i + 1 < len && (input[i + 1] == '?'
					|| ft_isalnum(input[i + 1]) || input[i
						+ 1] == '_'))
				new_expansion = expand_variable(tools, input, &i);
			else
			{
				if (input[i + 1] == '\0')
					new_expansion = ft_strdup("$");
				else
					new_expansion = ft_strdup("");
				i++;
			}
		}
		else
		{
			new_expansion = expand_plain_text(input, &i);
		}
		if (new_expansion && *new_expansion)
		{
			temp_word = ft_strjoin(expanded_word, new_expansion);
			free(expanded_word);
			free(new_expansion);
			expanded_word = temp_word;
		}
	}
	
	return(expanded_word);
}

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

void    redir_heredoc(t_simple_cmds **current_cmd, t_lexical **redir,
        t_tools *tools)
{
    char    *input;
    int        fd;
    char    *ttname;
    char    *temp;
    char    *temp2;

    ttname = ttyname(1);
    temp = ft_substr(ttname, 9, ft_strlen(ttname));
    temp2 = ft_strjoin("/tmp/", temp);
    free(temp);
    ttname = temp2;

    *redir = (*redir)->next;
    if ((*current_cmd)->num_redirections_heredoc > 16)
    {
        printf("minishell: maximum here-document count exceeded is %d\n",
            (*current_cmd)->num_redirections_heredoc);
        free(ttname);
        exit(2);
    }
    if ((*current_cmd)->fd_in != 0 && (*current_cmd)->fd_in != -1)
        close((*current_cmd)->fd_in);
    fd = open(ttname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell");
        free(ttname);
        return;
    }
    while (1)
    {
        input = readline("> ");
        if (!input)
            break;
        if (strcmp(input, (*redir)->str) == 0)
        {
            free(input);
            break;
        }
        if ((*redir)->filename_flag == 2)
        {
            char *expanded = expand_inside_heredoc(tools, input);
            free(input);
            input = expanded;
        }
        if (!input)
            input = ft_strdup("");
        write(fd, input, strlen(input));
        write(fd, "\n", 1);
        free(input);
    }
    close(fd);
    (*current_cmd)->fd_in = open(ttname, O_RDONLY);
    if ((*current_cmd)->fd_in == -1)
        perror("minishell");
    if (unlink(ttname) == -1)
        perror("minishell: failed to remove heredoc temp file");
    free(ttname);
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
