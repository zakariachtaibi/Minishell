/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/25 11:01:23 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_command(t_lexical **temp, t_simple_cmds **current_cmd,
							t_simple_cmds **cmds_head)
{
	t_simple_cmds	*new_cmd;

	if ((*current_cmd == NULL) || ((*temp)->token == TOKEN_PIPE))
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

int	check_token(t_lexical *temp)
{
	if ((temp)->token == TOKEN_REDIRECT_IN
		|| (temp)->token == TOKEN_REDIRECT_OUT
		|| (temp)->token == TOKEN_HEREDOC
		|| (temp)->token == TOKEN_APPEND)
		return (1);
	else
		return (0);
}

void	handle_redirections(t_lexical **temp, t_simple_cmds **current_cmd,
								t_lexical *token)
{
	t_lexical	*redir;
	t_lexical	*filename;

	if (check_token(*temp))
	{
		redir = copy_node(*temp);
		add_redirection((&(*current_cmd)->redirections), redir);
		(*current_cmd)->num_redirections++;
		delete_node(&token, *temp);
		*temp = (*temp)->next;
		if (*temp && (*temp)->token == TOKEN_WORD)
		{
			if (redir->token == TOKEN_HEREDOC)
				(*current_cmd)->hd_file_name = strdup((*temp)->str);
			else
			{
				filename = copy_node(*temp);
				add_redirection((&(*current_cmd)->redirections), filename);
			}
			delete_node(&token, *temp);
			*temp = (*temp)->next;
		}
	}
}

void	handle_words(t_lexical **temp, t_simple_cmds *current_cmd, t_tools *tools)
{
	int			word_count;
	t_lexical	*word_temp;
	int			i;

	word_temp = *temp;
	word_count = 0;
	while (word_temp && word_temp->token == TOKEN_WORD)
	{
		word_count++;
		word_temp = word_temp->next;
	}
	current_cmd->str = malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	while (*temp && (*temp)->token == TOKEN_WORD)
	{
		current_cmd->str[i] = expand_vars(tools, (*temp));
		*temp = (*temp)->next;
		i++;
	}
	current_cmd->str[i] = NULL;
}

t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
	t_simple_cmds	*cmds_head;
	t_simple_cmds	*current_cmd;
	t_lexical		*temp;

	cmds_head = NULL;
	current_cmd = NULL;
	temp = tokens;
	while (temp)
	{
		process_command(&temp, &current_cmd, &cmds_head);
		handle_redirections(&temp, &current_cmd, tokens);
		handle_words(&temp, current_cmd, tools);
		check_and_set_builtin(current_cmd);
	}
	return(cmds_head);
}
