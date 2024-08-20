/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:55:53 by hchouai           #+#    #+#             */
/*   Updated: 2024/08/20 13:06:06 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>

typedef enum e_token
{
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_WORD
}	t_token;

typedef enum e_builtin 
{
    BUILTIN_ECHO,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_EXPORT,
    BUILTIN_UNSET,
    BUILTIN_ENV,
    BUILTIN_EXIT,
    BUILTIN_NONE
} t_builtin;

typedef struct s_lexical
{
	char				*str;
	t_token				token;
	struct s_lexical	*next;
	struct s_lexical	*prev;
	int					i;
}	t_lexical;

typedef struct s_env_var
{
    char *key;
    char *value;
    struct s_env_var *next;
} t_env_var;

typedef struct s_tools
{
	t_env_var	*env_vars;
	size_t		var_start;
	size_t		var_len;
	char		*var_name;
	char		*var_value;
	char		*working_dir_path;
	int			exit_status;
}	t_tools;

typedef struct s_simple_cmds
{
	char					**str;
	int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	int						num_redirections;
	char					*hd_file_name;
	t_lexical				*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
	int						fd_in;
	int						fd_out;
}	t_simple_cmds;

void			count_quotes_and_parentheses(char c, int *quote, int *dquote, int *parentheses);
int				has_unclosed_quotes_or_parentheses(char *temp);
int				has_invalid_redirections(t_lexical *tokens);
void			validat_input(char *input);
t_lexical		*validate_syntax(t_lexical *tokens);
t_simple_cmds	*init_cmd(void);
t_lexical		*tokenize(char *input);
t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools);
void			add_redirection(t_lexical **redirections, t_lexical		*redir_node);
t_simple_cmds	*init_cmd(void);
t_lexical		*copy_node(t_lexical *src);
void			delete_node(t_lexical **head, t_lexical *node_to_delete);
char			*remove_enclosing_chars(char *input);
t_builtin		check_builtins_type(char *str);
void			check_and_set_builtin(t_simple_cmds *cmd);
int				(*get_builtin_func(t_builtin type))(t_tools *tools, t_simple_cmds *cmds);
int				builtin_echo(t_tools *tools, t_simple_cmds *cmd);
int				builtin_cd(t_tools *tools, t_simple_cmds *cmd);
int 			builtin_pwd(t_tools *tools, t_simple_cmds *cmd);
int 			builtin_export(t_tools *tools, t_simple_cmds *cmd);
int 			builtin_unset(t_tools *tools, t_simple_cmds *cmd);
int 			builtin_env(t_tools *tools, t_simple_cmds *cmd);
int 			builtin_exit(t_tools *tools, t_simple_cmds *cmd);
void			execute_commands(t_simple_cmds *cmds_head, t_tools **tools);
void			get_env_vars(t_tools *tools, char **envp);
char			*expand_vars(t_tools *tools, t_lexical *temp);
void			delete_node_env(t_env_var **head, t_env_var *node_to_delete);
char			*ft_strndup(const char *src, size_t n) ;
void			print_sorted_env(t_tools *tools);
char 			*get_env_value(t_env_var *env_vars, const char *key);
void			handle_env_var(t_tools *tools, char *key, char *value);
void 			add_new_env_var(t_tools *tools, char *key, char *value,
t_env_var		*current, t_env_var *prev);
void			check_and_set_redirections(t_simple_cmds *current_cmd);
void			execute_command(t_simple_cmds *cmd);
void 			execute_cmd(t_simple_cmds *current_cmd, t_tools **tools);
void			search_for_argn(t_simple_cmds *cmd, int *flag, int *j);
int				is_numeric(char *str);
void			process_export(t_tools *tools, char **str, int *i);

#endif