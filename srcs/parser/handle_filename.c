/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_filename.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:33:47 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/15 22:33:49 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	copy_and_prepare_filename(t_tools *tools, t_lexical **temp,
		t_lexical **filename)
{
	char	*expanded;

	*filename = copy_node(*temp);
	if (!(*filename))
		return (1);
	if (!(ft_strchr((*filename)->str, '"')) && !(ft_strchr((*filename)->str,
				'\'')))
		(*filename)->filename_flag = 2;
	else
		(*filename)->filename_flag = 0;
	expanded = expand_vars(tools, (*filename)->str);
	if (!expanded)
		return (1);
	free((*filename)->str);
	(*filename)->str = expanded;
	return (0);
}

static int	finalize_and_add_filename(t_tools *tools, t_lexical **temp,
		t_simple_cmds *current_cmd, t_lexical *filename)
{
	char	*unescaped;

	unescaped = unescape_spaces(filename->str, tools->flag);
	if (!unescaped)
	{
		free_lexical_node(filename);
		return (1);
	}
	free(filename->str);
	filename->str = ft_strdup(unescaped);
	free(unescaped);
	add_redirection(&current_cmd->redirections, filename);
	*temp = (*temp)->next;
	return (0);
}

int	handle_filename(t_tools **tools, t_lexical **temp,
		t_simple_cmds **current_cmd, int heredoc_flag)
{
	t_lexical	*filename;

	(void)heredoc_flag;
	(*tools)->flag = 0;
	if (*temp && (*temp)->token == TOKEN_WORD)
	{
		if (copy_and_prepare_filename(*tools, temp, &filename))
			return (1);
		if (finalize_and_add_filename(*tools, temp, *current_cmd, filename))
			return (1);
	}
	return (0);
}

int	copy_and_flag_filename(t_tools *tools, t_lexical **temp,
		t_lexical **filename)
{
	(void)*tools;
	*filename = copy_node(*temp);
	if (!(*filename))
		return (1);
	if (!(ft_strchr((*filename)->str, '"')) && !(ft_strchr((*filename)->str,
				'\'')))
		(*filename)->filename_flag = 2;
	else
		(*filename)->filename_flag = 0;
	return (0);
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
