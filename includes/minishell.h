/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:55:53 by hchouai           #+#    #+#             */
/*   Updated: 2024/10/30 13:11:35 by hchouai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

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
}	t_builtin;

typedef struct s_lexical
{
	char				*str;
	t_token				token;
	struct s_lexical	*next;
	struct s_lexical	*prev;
	int					i;
	int					filename_flag;
}	t_lexical;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_tools
{
	t_env_var	*env_vars;
	size_t		var_start;
	size_t		var_len;
	char		*var_name;
	char		*var_value;
	char		*working_dir_path;
	int			exit_status;
	int			std_out;
	int			std_in;
	int			export_flag;
}	t_tools;

typedef struct s_simple_cmds
{
	char					**str;
	int						(*builtin)(t_tools *, struct s_simple_cmds *);
	int						num_redirections;
	int						num_redirections_heredoc;
	char					*hd_file_name;
	int						count_heredocs;
	t_lexical				*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
	int						fd_in;
	int						fd_out;
}	t_simple_cmds;

int				(*get_builtin_func(t_builtin type))(t_tools *tools,
					t_simple_cmds *cmds);
void			get_env_vars(t_tools *tools, char **envp);
char			*get_env_value(t_env_var *env_vars, const char *key);
char			**get_path_dirs(t_env_var *env_vars);
char			*get_env_value(t_env_var *env_vars, const char *key);
char			*get_vars_value(char *str, t_tools *tools);
t_simple_cmds	*init_cmd(void);
char			**allocate_envp_array(int count);
void			execute_commands(t_simple_cmds *cmds_head, t_tools **tools,
					t_lexical *tokens);
void			execute_cmd(t_simple_cmds *current_cmd, t_tools **tools);
int				execute_if_absolute_path(t_simple_cmds *current_cmd,
					t_tools **tools);
void			execute(char *cmd_path, t_simple_cmds *current_cmd,
					t_tools **tools);
int				execute_from_path(char **split, t_simple_cmds *current_cmd,
					t_tools **tools);
int				builtin_echo(t_tools *tools, t_simple_cmds *cmd);
int				builtin_cd(t_tools *tools, t_simple_cmds *cmd);
int				builtin_pwd(t_tools *tools, t_simple_cmds *cmd);
int				builtin_export(t_tools *tools, t_simple_cmds *cmd);
int				builtin_unset(t_tools *tools, t_simple_cmds *cmd);
int				builtin_env(t_tools *tools, t_simple_cmds *cmd);
int				builtin_exit(t_tools *tools, t_simple_cmds *cmd);
t_builtin		check_builtins_type(char *str);
void			check_and_set_builtin(t_simple_cmds *cmd);
void			check_and_set_redirections(t_simple_cmds *current_cmd,
					t_tools **tools);
int				check_cd_arguments(t_tools *tools, t_simple_cmds *cmd);
char			*validat_input(char *input, t_tools *tools);
t_lexical		*validate_syntax(t_lexical *tokens, t_tools *tools);
int				has_unclosed_quotes_or_parentheses(char *temp, t_tools *tools);
int				has_invalid_redirections(t_lexical **tokens);
int				has_semicolon(char *input, t_tools *tools);
int				is_numeric(char *str);
void			add_redirection(t_lexical **redirections,
					t_lexical *redir_node);
void			add_new_env_var(t_tools *tools, char *key, char *value,
					t_env_var *current, t_env_var *prev);
t_lexical		*copy_node(t_lexical *src);
t_env_var		*copy_env_vars(t_env_var *env_vars);
void			delete_node(t_lexical **head, t_lexical *node_to_delete);
void			delete_node_env(t_env_var **head, t_env_var *node_to_delete);
char			*remove_enclosing_chars(char *input);
t_simple_cmds	*process_tokens(t_lexical *tokens, t_tools *tools);		
void			process_export(t_tools *tools, char **str, int *i);
void			count_quotes_and_parentheses(char c, int *quote, int *dquote,
					int *parentheses);
int				count_env_vars(t_env_var *env_vars);
void			print_sorted_env(t_tools *tools);
void			print_env_vars(t_env_var *copy);
void			handle_env_var(t_tools **tools, char *key, char *value);
void			handle_command_not_found(t_simple_cmds *current_cmd,
					t_tools **tools);
t_lexical		*tokenize(char *input);
char			*expand_vars(t_tools *tools, char *temp, int *flag,
					int heredoc_flag);
char			*expand_double_quote(t_tools *tools, const char *current_word,
					size_t *j, int heredoc_flag);
char			*expand_plain_text(const char *current_word, size_t *j);
char			*expand_variable(t_tools *tools, const char *current_word,
					size_t *j);
char			*ft_strndup(const char *src, size_t n);
void			search_for_argn(t_simple_cmds *cmd, int *flag, int *j);
int				change_directory(t_tools *tools, t_simple_cmds *cmd);
void			update_pwd_variables(t_tools *tools);
void			fill_envp_array(char **envp, t_env_var *env_vars, int count);
char			**convert_env_vars_to_array(t_env_var *env_vars);
void			sort_env_vars(t_env_var *copy);	
void			free_env_vars(t_env_var *copy);
void			setup_signal(void);
void			handle_sigint(int sig);
void			sigint2(void);
void			sig_handler1(int test);
void			free_lexical(t_lexical *head);
void			swap_values(char **ikey, char **ivalue, char **jkey,
					char **jvalue);
void			check_env_vars(t_env_var **current);
void			ft_free(char **arr);
void			free_cmds(t_simple_cmds **cmds);
void			free_tools(t_tools *tools);
void			cleanup_readline(void);
int				is_valid_identifier(const char *str);
void			increment_SHLVL(t_tools **tools);
t_tools			*init_tools(void);

#endif