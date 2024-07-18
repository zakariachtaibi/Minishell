/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/18 16:17:18 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_redirection(t_lexical **redirections, t_lexical *redir_node)
{
	t_lexical	*current;

	if (*redirections == NULL)
		*redirections = redir_node;
	else
	{
		current = *redirections;
		while (current->next != NULL)
			current = current->next;
		current->next = redir_node;
		redir_node->prev = current;
	}
	redir_node->next = NULL;
}

t_simple_cmds	*init_cmd(void)
{
	t_simple_cmds	*new_cmd ;

	new_cmd = malloc(sizeof(t_simple_cmds));
	new_cmd->next = NULL;
	new_cmd->prev = NULL;
	new_cmd->str = NULL;
	new_cmd->num_redirections = 0;
	new_cmd->hd_file_name = NULL;
	new_cmd->redirections = NULL;
	return (new_cmd);
}

t_lexical	*copy_node(t_lexical *src)
{
	t_lexical	*dest;

	if (src == NULL)
		return (NULL);
	dest = (t_lexical *)malloc(sizeof(t_lexical));
	if (dest == NULL)
		return (NULL);
	dest->token = src->token;
	dest->i = src->i;
	dest->next = NULL;
	dest->prev = NULL;
	if (src->str != NULL)
	{
		dest->str = strdup(src->str);
		if (dest->str == NULL)
		{
			free(dest);
			return (NULL);
		}
	}
	else
		dest->str = NULL;
	return (dest);
}

void	delete_node(t_lexical **head, t_lexical *node_to_delete)
{
	if (node_to_delete == NULL)
		return ;
	if (*head == node_to_delete)
		*head = node_to_delete->next;
	if (node_to_delete->next != NULL)
		node_to_delete->next->prev = node_to_delete->prev;
	if (node_to_delete->prev != NULL)
		node_to_delete->prev->next = node_to_delete->next;
	free(node_to_delete->str);
	free(node_to_delete);
}

void	process_command(t_lexical **temp, t_simple_cmds **current_cmd, t_simple_cmds **cmds_head)
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

void	handle_redirections(t_lexical **temp, t_simple_cmds **current_cmd, t_lexical *token)
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

void	handle_words(t_lexical **temp, t_simple_cmds *current_cmd)
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
		current_cmd->str[i] = strdup((*temp)->str);
		*temp = (*temp)->next;
		i++;
	}
	current_cmd->str[i] = NULL;
}

void	process_tokens(t_lexical *tokens)
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
		handle_words(&temp, current_cmd);
	}
}
