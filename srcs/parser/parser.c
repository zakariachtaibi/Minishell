/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 20:01:32 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int	check_token(t_lexical *temp, int *heredoc_flag)
{
	if ((temp)->token == TOKEN_REDIRECT_IN
		|| (temp)->token == TOKEN_REDIRECT_OUT || (temp)->token == TOKEN_HEREDOC
		|| (temp)->token == TOKEN_APPEND)
	{
		if ((temp)->token == TOKEN_HEREDOC)
			*heredoc_flag = 1;
		return (1);
	}
	else
		return (0);
}

int	handle_filename(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, int heredoc_flag)
{
	t_lexical	*filename;
	char		*expanded;
	char		*unescaped;

	(void)heredoc_flag;
	(*tools)->flag = 0;
	if (*temp && (*temp)->token == TOKEN_WORD)
	{
		filename = copy_node(*temp);
		if (!(ft_strchr(filename->str, '"')) && !(ft_strchr(filename->str,
					'\'')))
			filename->filename_flag = 2;
		else
			filename->filename_flag = 0;
		expanded = expand_vars((*tools), filename->str);
		free(filename->str);
		filename->str = expanded;
		unescaped = unescape_spaces(filename->str, (*tools)->flag);
		if (!unescaped)
		{
			free_lexical_node(filename);
			return (1);
		}
		free(filename->str);
		filename->str = ft_strdup(unescaped);
		free(unescaped);
		add_redirection((&(*current_cmd)->redirections), filename);
		*temp = (*temp)->next;
	}
	return (0);
}

int	handle_redirections(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, t_lexical *token)
{
	t_lexical	*redir;
	t_lexical	*filename;
	char		*unescaped;
	t_lexical	*node_todel;
	char		*expanded;

	(void)token;
	(*tools)->flag = 0;
	(*tools)->heredoc_flag = 0;
	if (check_token(*temp, &(*tools)->heredoc_flag))
	{
		redir = copy_node(*temp);
		add_redirection((&(*current_cmd)->redirections), redir);
		(*current_cmd)->num_redirections++;
		if ((*tools)->heredoc_flag == 1)
			(*current_cmd)->num_redirections_heredoc++;
		node_todel = copy_node(*temp);
		*temp = (*temp)->next;
		delete_node(&token, node_todel);
		if (*temp && (*temp)->token == TOKEN_WORD)
		{
			filename = copy_node(*temp);
			if (!(ft_strchr(filename->str, '"')) && !(ft_strchr(filename->str,
						'\'')))
				filename->filename_flag = 2;
			else
				filename->filename_flag = 0;
			expanded = expand_vars((*tools), filename->str);
			free(filename->str);
			filename->str = expanded;
			unescaped = unescape_spaces(filename->str, (*tools)->flag);
			if (!unescaped)
			{
				free_lexical_node(filename);
				return (1);
			}
			free(filename->str);
			filename->str = ft_strdup(unescaped);
			free(unescaped);
			add_redirection((&(*current_cmd)->redirections), filename);
			*temp = (*temp)->next;
		}
	}
	return (0);
}

t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
	t_simple_cmds	*cmds_head;
	t_simple_cmds	*current_cmd;
	t_lexical		*temp;

	tools->flag = 0;
	tools->heredoc_flag = 0;
	cmds_head = NULL;
	current_cmd = NULL;
	temp = tokens;
	while (temp)
	{
		process_command(&temp, &current_cmd, &cmds_head);
		while (temp && !check_token(temp, &tools->heredoc_flag)
			&& temp->token != TOKEN_PIPE)
			handle_words(&temp, &current_cmd, tools);
		current_cmd->num_redirections_heredoc = 0;
		while (temp && check_token(temp, &tools->heredoc_flag))
		{
			tools->flag = handle_redirections(&tools, &temp, &current_cmd,
					tokens);
			if (tools->flag == 1)
			{
				ft_putstr_fd((temp)->str, 2);
				ft_putstr_fd(" : ambiguous redirect\n", 2);
				tools->exit_status = 1;
				free_cmds(&current_cmd);
				return (NULL);
			}
		}
		check_and_set_redirections(current_cmd, &tools);
		if ((((current_cmd)->fd_out == -1) || ((current_cmd)->fd_in == -1)))
		{
			ft_putstr_fd("Minishell : No such file or directory\n", 2);
			tools->exit_status = 1;
			free_cmds(&current_cmd);
			return (NULL);
		}
		while (temp && !check_token(temp, &tools->heredoc_flag)
			&& temp->token != TOKEN_PIPE)
			handle_words(&temp, &current_cmd, tools);
		check_and_set_builtin(current_cmd);
	}
	return (cmds_head);
}
