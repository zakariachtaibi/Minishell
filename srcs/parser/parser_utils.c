/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:19:56 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/24 11:33:32 by hchouai          ###   ########.fr       */
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

char *ft_strndup(const char *src, size_t n) 
{
    size_t len = ft_strlen(src);
    if (len > n) len = n;  

    char *dup = (char *)malloc(len + 1);  
    if (!dup) return NULL;  

    ft_strlcpy(dup, src, len); 
    dup[len] = '\0';  

    return dup;
}
