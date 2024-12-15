/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:30:41 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 22:29:46 by hchouai          ###   ########.fr       */
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
		(*current_cmd)->fd_out = open((*redir)->str,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		redir = redir->next;
	}
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
