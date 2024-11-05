/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:11:36 by hchouai           #+#    #+#             */
/*   Updated: 2024/11/05 16:07:20 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool	is_quote(char c) {
    return (c == '"' || c == '\'');
}

int count_words(const char *str) {
    int count = 0;
    bool in_word = false;
    bool inside_quotes = false;
    char quote_char = '\0';

    while (*str) {
        if (is_quote(*str) && !inside_quotes) {
            inside_quotes = true;
            quote_char = *str;
            if (!in_word) {
                in_word = true; // Starting a new word
            }
        } else if (*str == quote_char && inside_quotes) {
            inside_quotes = false;
            quote_char = '\0';
        } else if (*str == ' ' && !inside_quotes) {
            if (in_word) {
                count++;
                in_word = false; // End of the current word
            }
        } else if (!in_word) {
            in_word = true; // Found a new word
        }
        str++;
    }
    if (in_word) {
        count++; // Count the last word if any
    }
    return count;
}

char *extract_word(const char **str) {
    const char *start = *str;
    bool inside_quotes = false;
    char quote_char = '\0';
    int length = 0;

    // Move through the word, handling quoted sections as needed
    while (**str && (**str != ' ' || inside_quotes)) {
        if (is_quote(**str) && !inside_quotes) {
            inside_quotes = true;
            quote_char = **str;
        } else if (**str == quote_char && inside_quotes) {
            inside_quotes = false;
            quote_char = '\0';
        }
        (*str)++;
        length++;
    }

    char *word = malloc(length + 1);
    if (!word) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(word, start, length);
    word[length] = '\0';

    // Skip trailing spaces after the word
    while (**str == ' ')
        (*str)++;
    return word;
}

char **split_ignoring_quotes(const char *str) {
    int word_count = count_words(str);
    char **result = malloc(sizeof(char *) * (word_count + 1));
    if (!result) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (*str) {
        while (*str == ' ')
            str++; // Skip initial spaces
        if (*str) {
            result[i++] = extract_word(&str); // Extract the next word
        }
    }
    result[i] = NULL; // Null-terminate the array

    return result;
}

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	is_space(char *str)
{
	int	i;
	i = 0;
	while (str[i] != '\0' && str[i] != '"')
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

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

char	*unescape_spaces(char *str, int flag)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str || str[0] == '\0')
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	if (!str || str[0] == '\0')
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ' && flag == 1)
		{
			free(result);
			return (NULL);
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int	handle_redirections(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, t_lexical *token)
{
	t_lexical	*redir;
	t_lexical	*filename;
	char		*unescaped;
	int			flag;
	int			heredoc_flag;

	flag = 0;
	heredoc_flag = 0;
	(*current_cmd)->num_redirections_heredoc = 0;
	if (check_token(*temp, &heredoc_flag))
	{
		redir = copy_node(*temp);
		add_redirection((&(*current_cmd)->redirections), redir);
		(*current_cmd)->num_redirections++;
		if (heredoc_flag == 1)
			(*current_cmd)->num_redirections_heredoc++;
		delete_node(&token, *temp);
		*temp = (*temp)->next;
		if (*temp && (*temp)->token == TOKEN_WORD)
		{
			filename = copy_node(*temp);
			if (!(ft_strchr(filename->str, '"')) && !(ft_strchr(filename->str,
						'\'')))
				filename->filename_flag = 2;
			filename->str = expand_vars((*tools), filename->str, &flag,
					heredoc_flag);
			unescaped = unescape_spaces(filename->str, flag);
			if (!unescaped)
			{
				free(redir); // Free redirection if unescape fails
				free(filename->str); // Free the string before exiting
				free(filename);      // Free the node itself
				return (1);
			}

			free(filename->str); // Free the old string
			filename->str = ft_strdup(unescaped); // Assign unescaped string
			free(unescaped); // Free the temporary unescaped string

			add_redirection((&(*current_cmd)->redirections), filename);  // Store filename
			delete_node(&token, *temp);
			*temp = (*temp)->next;
		}
	}
	return (0);
}

int	ft_strlen_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

char *unescape_quotes(char *str)
{
    int     length;
	int     i;
    int     j;
    char    *result;
	
	length = 0;
	i = -1;
    if (!str)
	 return (NULL);
	while (str[++i])
	{
		if (str[i] != '"')
            length++;
	}
   result = malloc(length + 1);
    if (!result)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    j = 0;
    i = -1;
    while (str[++i])
    {
        if (str[i] != '"' && str[i] != '\'')
            result[j++] = str[i];
    }
    result[j] = '\0';
    return (result);
}

void	handle_words(t_lexical **temp, t_simple_cmds **current_cmd, t_tools *tools)
{
    int word_count;
    t_lexical *word_temp;
    int i;
    int count_str;
    char **new_str;
    char *expanded;
    int flag;
    char **tmp;
    int in;
    int heredoc_flag;
    int export_flag;
    char *cleaned;

    flag = 0;
    in = 0;
    heredoc_flag = 0;
    export_flag = 0;
    word_temp = *temp;
    word_count = 0;

    while (word_temp && word_temp->token == TOKEN_WORD)
    {
        word_count++;
        word_temp = word_temp->next;
    }
    if ((*current_cmd)->str)
        count_str = ft_strlen_array((*current_cmd)->str);
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
        new_str[i] = (*current_cmd)->str[i];
    free((*current_cmd)->str);
    (*current_cmd)->str = new_str;
    i = count_str;
    while (*temp && (*temp)->token == TOKEN_WORD)
    {
        if (strcmp((*temp)->str, "export") == 0)
            export_flag = 1;
        expanded = expand_vars(tools, (*temp)->str, &flag, heredoc_flag);
        tools->export_flag = flag;
        if (expanded)
        {
            // Ignore quotes in the expanded string
            cleaned = unescape_quotes(expanded);
            free(expanded);
            if ((is_space(cleaned) == 1) && (export_flag != 1))
            {
                in = 0;
                tmp = split_ignoring_quotes(cleaned);
                while (tmp[in])
                {
                    if (i >= (word_count + count_str))
                    {
                        int new_size = (word_count + count_str) * 2; // Double the size
                        new_str = realloc(new_str, sizeof(char *) * (new_size + 1));
                        if (!new_str)
                        {
                            perror("realloc");
                            exit(EXIT_FAILURE);
                        }
                        (*current_cmd)->str = new_str;
                        word_count = new_size - count_str; 
                    }
                    (*current_cmd)->str[i] = ft_strdup(tmp[in]);
                    in++;
                    i++;
                }
                free(cleaned);
                ft_free(tmp);
            } else
            {
                if (i >= (word_count + count_str))
                {
                    int new_size = (word_count + count_str) * 2; // Double the size
                    new_str = realloc(new_str, sizeof(char *) * (new_size + 1));
                    if (!new_str)
                    {
                        perror("realloc");
                        exit(EXIT_FAILURE);
                    }
                    (*current_cmd)->str = new_str;
                    word_count = new_size - count_str; 
                }
                (*current_cmd)->str[i] = cleaned;
                i++;
            }
        }
        *temp = (*temp)->next;
    }
    (*current_cmd)->str[i] = NULL;
}

t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools)
{
	t_simple_cmds	*cmds_head;
	t_simple_cmds	*current_cmd;
	t_lexical		*temp;
	int				flag;
	int				h_flag;

	flag = 0;
	h_flag = 0;
	cmds_head = NULL;
	current_cmd = NULL;
	temp = tokens;
	while (temp)
	{
		process_command(&temp, &current_cmd, &cmds_head);
		while (temp && !check_token(temp, &h_flag) && temp->token != TOKEN_PIPE)
			handle_words(&temp, &current_cmd, tools);
		while (temp && check_token(temp, &h_flag))
		{
			flag = handle_redirections(&tools, &temp, &current_cmd, tokens);
			if (flag == 1)
			{
				printf("%s : ambiguous redirect\n", (temp)->str);
				tools->exit_status = 1;
				return (NULL);
			}
		}
		check_and_set_redirections(current_cmd, &tools);
		if ((((current_cmd)->fd_out == -1) || ((current_cmd)->fd_in == -1)))
		{
			perror("minishell");
			tools->exit_status = 1;
			return (NULL);
		}
		while (temp && !check_token(temp, &h_flag) && temp->token != TOKEN_PIPE)
			handle_words(&temp, &current_cmd, tools);
		check_and_set_builtin(current_cmd);
	}
	return (cmds_head);
}
