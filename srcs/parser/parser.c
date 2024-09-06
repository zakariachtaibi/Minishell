/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/09/06 12:32:16 by hchouai          ###   ########.fr       */
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

char *unescape_spaces(char *str)
{
    char 	*result;
    int 	i = 0;
	int 	j = 0;

    result = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (!result)
        return (NULL);
    while (str[i])
    {
        if (str[i] == '\\' && str[i + 1] == ' ')
            i++;
        result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

int handle_redirections(t_tools **tools, t_lexical **temp, t_simple_cmds **current_cmd, t_lexical *token)
{
    t_lexical *redir;
    t_lexical *filename;
	char	*unescaped;

    if (check_token(*temp))
    {
        redir = copy_node(*temp);
        add_redirection((&(*current_cmd)->redirections), redir);
        (*current_cmd)->num_redirections++;
        delete_node(&token, *temp);
        *temp = (*temp)->next;

        if (*temp && (*temp)->token == TOKEN_WORD)
        {
            filename = copy_node(*temp);
            filename->str = expand_vars((*tools), filename);
            unescaped = unescape_spaces(filename->str);
            if (!unescaped)
            {
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
            free(filename->str);
            filename->str = unescaped;
            add_redirection((&(*current_cmd)->redirections), filename);
            delete_node(&token, *temp);
            *temp = (*temp)->next;
        }
    }
    return 0;  // No ambiguity detected
}


int ft_strlen_array(char **array)
{
    int i = 0;
    while (array && array[i])
        i++;
    return i;
}
void	handle_words(t_lexical **temp, t_simple_cmds *current_cmd, t_tools *tools)
{
	int			word_count;
	t_lexical	*word_temp;
	int			i;
	int			count_str;
	char		**new_str;

	word_temp = *temp;
	word_count = 0;
	while (word_temp && word_temp->token == TOKEN_WORD)
	{
		word_count++;
		word_temp = word_temp->next;
	}
	if (current_cmd->str)
		count_str = ft_strlen_array(current_cmd->str);
	else
		count_str = 0;

	new_str = malloc(sizeof(char *) * (word_count + count_str + 1));
	if (!new_str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = -1;
    while (++i < count_str)
    {
        new_str[i] = current_cmd->str[i];
    }
	free(current_cmd->str);
	current_cmd->str = new_str;
	i = count_str;
	while (*temp && (*temp)->token == TOKEN_WORD)
	{
		current_cmd->str[i] = expand_vars(tools, (*temp));
		*temp = (*temp)->next;
        i++;
	}
	current_cmd->str[i]= NULL;
}


t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
    t_simple_cmds	*cmds_head;
    t_simple_cmds	*current_cmd;
    t_lexical		*temp;
    int				flag = 0;

    cmds_head = NULL;
    current_cmd = NULL;
    temp = tokens;
    while (temp)
    {
        process_command(&temp, &current_cmd, &cmds_head);
        while (temp && !check_token(temp) && temp->token != TOKEN_PIPE)
            handle_words(&temp, current_cmd, tools);
        while (temp && check_token(temp))
        {
            flag = handle_redirections(&tools, &temp, &current_cmd, tokens);
            if (flag == 1)
            {
                printf("$filename: ambiguous redirect\n");
                tools->exit_status = 1;
                return NULL;
            }
            check_and_set_redirections(current_cmd);
        }
        while (temp && !check_token(temp) && temp->token != TOKEN_PIPE)
            handle_words(&temp, current_cmd, tools);
        check_and_set_builtin(current_cmd);
    }
    return cmds_head;
}

