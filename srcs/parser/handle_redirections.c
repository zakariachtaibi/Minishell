/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 13:16:24 by mac               #+#    #+#             */
/*   Updated: 2024/12/14 19:15:30 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_redirection_flags(t_tools *tools, t_lexical *temp)
{
	tools->flag = 0;
	tools->heredoc_flag = 0;
	return (check_token(temp, &tools->heredoc_flag));
}

static int	process_redirection_node(t_tools **tools, t_lexical **temp,
		t_simple_cmds *current_cmd)
{
	t_lexical	*redir;
	t_lexical	*node_todel;

	redir = copy_node(*temp);
	if (!redir)
		return (1);
	add_redirection(&current_cmd->redirections, redir);
	current_cmd->num_redirections++;
	if ((*tools)->heredoc_flag == 1)
		current_cmd->num_redirections_heredoc++;
	node_todel = copy_node(*temp);
	if (!node_todel)
		return (1);
	*temp = (*temp)->next;
	delete_node(temp, node_todel);
	return (0);
}

static int	expand_and_unescape_filename(t_tools *tools, t_lexical *filename)
{
	char	*expanded;
	char	*unescaped;

	expanded = expand_vars(tools, filename->str);
	if (!expanded)
		return (1);
	free(filename->str);
	filename->str = expanded;
	unescaped = unescape_spaces(filename->str, tools->flag);
	if (!unescaped)
		return (1);
	free(filename->str);
	filename->str = ft_strdup(unescaped);
	free(unescaped);
	return (0);
}

static int	handle_fileename(t_tools *tools, t_lexical **temp,
		t_simple_cmds *current_cmd)
{
	t_lexical	*filename;

	if (copy_and_flag_filename(tools, temp, &filename))
		return (1);
	if (expand_and_unescape_filename(tools, filename))
	{
		free_lexical_node(filename);
		return (1);
	}
	add_redirection(&current_cmd->redirections, filename);
	*temp = (*temp)->next;
	return (0);
}

int	handle_redirections(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, t_lexical *token)
{
	(void)token;
	if (!handle_redirection_flags(*tools, *temp))
		return (0);
	if (process_redirection_node(tools, temp, *current_cmd))
		return (1);
	if (*temp && (*temp)->token == TOKEN_WORD)
	{
		if (handle_fileename(*tools, temp, *current_cmd))
			return (1);
	}
	return (0);
}
