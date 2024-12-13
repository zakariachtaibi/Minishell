/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:55:53 by hchouai           #+#    #+#             */
/*   Updated: 2024/12/13 14:35:23 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token
{
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_WORD
}							t_token;

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
}							t_builtin;

typedef struct s_lexical
{
	char					*str;
	t_token					token;
	struct s_lexical		*next;
	struct s_lexical		*prev;
	int						i;
	int						filename_flag;
}							t_lexical;
typedef struct s_tokenizer
{
	t_lexical				*head;
	t_lexical				*current;
	char					token[1024];
	int						i;
	int						j;
}							t_tokenizer;

typedef struct s_env_var
{
	char					*key;
	char					*value;
	struct s_env_var		*next;
}							t_env_var;

typedef struct s_tools
{
	t_env_var				*env_vars;
	size_t					var_start;
	size_t					var_len;
	char					*var_name;
	char					*var_value;
	char					*working_dir_path;
	int						exit_status;
	int						std_out;
	int						std_in;
	int						export_flag;
	int						flag;
	int						heredoc_flag;
}							t_tools;

typedef struct s_simple_cmds
{
	char					**str;
	int						(*builtin)(t_tools *, struct s_simple_cmds *,
			t_lexical *);
	int						num_redirections;
	int						num_redirections_heredoc;
	char					*hd_file_name;
	int						count_heredocs;
	t_lexical				*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
	int						fd_in;
	int						fd_out;
}							t_simple_cmds;

typedef struct s_exec
{
	int						pipe_fd[2];
	int						prev_pipe_read;
	int						status;
	t_simple_cmds			*current_cmd;
	pid_t					pid;
	pid_t					last_pid;
	int						e;
	int						builtin_executed;
	int						child_exit_status;
	t_simple_cmds			*cmds_head;
	t_tools					**tools;
	t_lexical				*tokens;
}							t_exec;

int	(*get_builtin_func(t_builtin type))(t_tools *tools, t_simple_cmds *cmds,
		t_lexical *tokens);
void						get_env_vars(t_tools *tools, char **envp);
char						*get_env_value(t_env_var *env_vars,
		const char *key);
char						**get_path_dirs(t_env_var *env_vars);
char						*get_env_value(t_env_var *env_vars,
		const char *key);
char						*get_vars_value(char *str, t_tools *tools);
t_simple_cmds				*init_cmd(void);
char						**allocate_envp_array(int count);
void						execute_commands(t_simple_cmds *cmds_head,
		t_tools **tools, t_lexical *tokens);
int							execute_command(t_simple_cmds *cmd, t_tools **tools,
		t_lexical *tokens);
void						execute_cmd(t_simple_cmds *current_cmd,
		t_tools **tools, t_lexical *tokens);
void						execute(char *cmd_path, t_simple_cmds *current_cmd,
		t_tools **tools);
void						handle_special_commands(t_simple_cmds *cmd,
		t_tools **tools, int *should_return);
int							builtin_echo(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_cd(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_pwd(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_export(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_unset(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_env(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
int							builtin_exit(t_tools *tools, t_simple_cmds *cmd,
		t_lexical *tokens);
t_builtin					check_builtins_type(char *str);
void						check_and_set_builtin(t_simple_cmds *cmd);
void						check_and_set_redirections(
		t_simple_cmds*current_cmd, t_tools **tools);
int							check_cd_arguments(t_tools *tools,
		t_simple_cmds *cmd);
char						*validat_input(char *input, t_tools *tools);
t_lexical					*validate_syntax(t_lexical *tokens, t_tools *tools);
int							has_unclosed_quotes_or_parentheses(char *temp,
		t_tools *tools);
int							has_invalid_redirections(t_lexical **tokens);
int							has_semicolon(char *input, t_tools *tools);
int							is_numeric(char *str);
void						add_redirection(t_lexical **redirections,
		t_lexical *redir_node);
void						add_new_env_var(t_tools *tools, char *key,
		char *value);
t_lexical					*copy_node(t_lexical *src);
t_env_var					*copy_env_vars(t_env_var *env_vars);
void						delete_node(t_lexical **head,
		t_lexical *node_to_delete);
void						delete_node_env(t_env_var **head,
		t_env_var *node_to_delete);
char						*remove_enclosing_chars(char *input);
t_simple_cmds				*process_tokens(t_lexical *tokens, t_tools *tools);
void						process_export(t_tools *tools, char **str, int *i);
void						count_quotes_and_parentheses(char c, int *quote,
		int *dquote, int *parentheses);
int							count_env_vars(t_env_var *env_vars);
void						print_sorted_env(t_tools *tools);
void						print_env_vars(t_env_var *copy);
void						handle_env_var(t_tools **tools, char *key,
		char *value);
void						handle_command_not_found(t_simple_cmds *current_cmd,
		t_tools **tools);
t_lexical					*tokenize(char *input);
t_lexical					*add_node_to_list(t_lexical **head,
		t_lexical **current, char *token, int i);
char						*expand_vars(t_tools *tools, char *temp);
char						*expand_double_quote(t_tools *tools,
		const char *current_word, size_t *j);
char						*expand_plain_text(const char *current_word,
		size_t *j);
char						*expand_variable(t_tools *tools,
		const char *current_word, size_t *j);
char						*ft_strndup(const char *src, size_t n);
void						search_for_argn(t_simple_cmds *cmd, int *flag,
		int *j);
int							change_directory(t_tools *tools,
		t_simple_cmds *cmd);
void						update_pwd_variables(t_tools *tools);
int							fill_envp_array(char **envp, t_env_var *env_vars,
		int count);
char						**convert_env_vars_to_array(t_env_var *env_vars);
void						sort_env_vars(t_env_var *copy);
void						free_env_vars(t_env_var *copy);
void						free_env_var(t_env_var *env_vars);
void						child_exit_cleanup(t_exec *ctx);
void						setup_signal(void);
void						handle_sigint(int sig);
void						sigint2(int sig);
void						sig_handler1(int test);
void						free_lexical(t_lexical *head);
void						swap_values(char **ikey, char **ivalue, char **jkey,
		char **jvalue);
void						check_env_vars(t_env_var **current);
void						ft_free(char **arr);
void						free_cmds(t_simple_cmds **cmds);
void						free_tools(t_tools *tools);
void						free_env_var(t_env_var *env_vars);
void						cleanup_readline(void);
int							is_valid_identifier(const char *str);
void						increment_shlvl(t_tools **tools);
t_tools						*init_tools(void);
int							is_space(char *str);
char						**split_ignoring_quotes(const char *str);
bool						is_quote(char c);
void						handle_sigint_child(int sig);
int							count_words(const char *str);
char						*extract_word(const char **str);
void						setup_child_signals(void);
void						parent_pipe_handle(t_exec *ctx);
void						init_execution_context(t_exec *ctx,
		t_simple_cmds *cmds_head, t_tools **tools,
		t_lexical *tokens);
void						wait_processes(t_exec *ctx);
char						*get_current_working_directory(t_tools *tools);
void						perform_exit_cleanup(t_simple_cmds *cmd,
		t_lexical *tokens, t_tools *tools);
int							validate_unset_identifier(char *identifier);
int							remove_env_variable(t_tools *tools, char *key);
int							env_error(t_tools *tools, char *arg);
void						check_valid_key(char *key, char *str, char *value,
		t_tools *tools);
void						print_error(char *str, char *key, char *value,
		t_tools *tools);
void						prints_error(char *cmd, char *msg,
		int *exit_status);
int							check_cd_arg_count(t_tools *tools,
		t_simple_cmds *cmd);
int							handle_home_path(t_tools *tools, char **path);
void						redir_heredoc(t_simple_cmds **current_cmd,
		t_lexical **redir,
		t_tools *tools);
int							ft_strlen_array(char **array);
char						*unescape_spaces(char *str, int flag);
void						free_lexical_node(t_lexical *node);
char						*get_vars(const char *current_word, size_t *j,
		t_tools *tools);
char						*expand_if_dollar(const char *current_word, char *expanded_word,
		size_t *j, t_tools *tools);
char						*expand_plain2(const char *current_word, char *expanded_word,
		size_t *j);
char						*expand_single_quote(const char *current_word, size_t *j);
char						*expand_variable(t_tools *tools, const char *current_word, size_t *j);
char						*handle_single_quote(const char *current_word, size_t *j, int *flag);
char						*handle_double_quote(t_tools *tools, const char *current_word,
		size_t *j);
char						*handle_dollar_sign(t_tools *tools, const char *current_word, size_t *j,
		size_t len);
t_env_var 					*create_env_var(const char *env_entry);
void 						append_env_var(t_env_var **head, t_env_var *new_var);
char						*expand_inside_heredoc(t_tools *tools, char *input);
char						*generate_temporary_filename(void);
char	*expand_var(char *input, size_t *i, t_tools *tools, size_t len);
void handle_words(t_lexical **temp, t_simple_cmds **current_cmd, t_tools *tools);
#endif