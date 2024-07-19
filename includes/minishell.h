/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:55:53 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/19 15:53:13 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token
{
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_WORD
}	t_token;

typedef struct s_lexical
{
	char				*str;
	t_token				token;
	struct s_lexical	*next;
	struct s_lexical	*prev;
	int					i;
}	t_lexical;

typedef struct s_simple_cmds
{
	char					**str;
	// int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	int						num_redirections;
	char					*hd_file_name;
	t_lexical				*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

int				has_unclosed_double_quotes(char *input);
int				has_mismatched_parentheses(char *temp);
int				has_invalid_redirections(t_lexical *tokens);
char			*validat_input(char *input);
t_simple_cmds	*init_cmd(void);
t_lexical		*tokenize(char *input);
void			process_tokens(t_lexical *tokens);
void			add_redirection(t_lexical **redirections,
t_lexical		*redir_node);
t_simple_cmds	*init_cmd(void);
t_lexical		*copy_node(t_lexical *src);
void			delete_node(t_lexical **head, t_lexical *node_to_delete);
#endif