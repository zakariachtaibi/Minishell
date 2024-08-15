/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/15 13:26:42 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	handle_redirections(t_tools **tools ,t_lexical **temp, t_simple_cmds **current_cmd,
								t_lexical *token)
{
	t_lexical	*redir;
	t_lexical	*filename;
	int			flag = 0;
	// t_lexical	*expanded_filename;

	// expanded_filename = NULL;
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
				(*current_cmd)->hd_file_name = ft_strdup((*temp)->str);
			else
			{
				filename = copy_node(*temp);
				filename->str= expand_vars((*tools), filename);
				add_redirection((&(*current_cmd)->redirections), filename);
				if(ft_strchr(filename->str, 32))
				flag = 1;
			}
			
			delete_node(&token, *temp);
			*temp = (*temp)->next;
		}
	}
	return (flag);
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

void	check_and_set_redirections(t_simple_cmds *current_cmd)
{
	t_lexical	*redir;
	int	std_out;
	std_out = 1;
	
	redir = current_cmd->redirections;
	while (redir)
	{
		if (redir->token == TOKEN_REDIRECT_IN)
		{
			current_cmd->fd_in = open(redir->str, O_RDONLY);
			if (current_cmd->fd_in == -1)
				perror("minishell");
		}
		else if (redir->token == TOKEN_REDIRECT_OUT)
		{
			redir = redir->next;
			current_cmd->fd_out = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(current_cmd->fd_out, std_out);
			close(current_cmd->fd_out);
			if (current_cmd->fd_out == -1)
				perror("minishell");
		}
		
		redir = redir->next;
	}
}

t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
	t_simple_cmds	*cmds_head;
	t_simple_cmds	*current_cmd;
	t_lexical		*temp;
	int flag = 0;

	cmds_head = NULL;
	current_cmd = NULL;
	temp = tokens;
	while (temp)
	{
		// expand_vars(tools, temp);
		process_command(&temp, &current_cmd, &cmds_head);
		while (temp && !check_token(temp) && temp->token != TOKEN_PIPE)
			handle_words(&temp, current_cmd, tools);
		while (temp && check_token(temp))
		{
			flag = handle_redirections(&tools ,&temp, &current_cmd, tokens);
			if(flag == 1)
			{
				printf("$filename: ambiguous redirect\n");
				tools->exit_status = 1;
				return(NULL);
			}
			check_and_set_redirections(current_cmd);
		}
		check_and_set_builtin(current_cmd);
	}
	return(cmds_head);
}
