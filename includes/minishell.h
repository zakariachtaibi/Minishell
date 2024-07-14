#ifndef MINISHELL_H

#define MINISHELL_H

<<<<<<< HEAD

=======
>>>>>>> 9b266628a3f111a29f39f6f4e86aba2cfa59a470
typedef enum e_token
{
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_HEREDOC,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_WORD
} t_token;

typedef struct s_lexical
{
    char *str;
    t_token token;
    struct s_lexical *next;
    struct s_lexical *prev;
    int i;
}   t_lexical;

typedef struct s_simple_cmds
{
	char                    **str;
	// int                     (*builtin)(t_tools *, struct s_simple_cmds *);
	int                     num_redirections;
	char                    *hd_file_name;
	t_lexical               *redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}	t_simple_cmds;

# include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

<<<<<<< HEAD
=======
// t_lexical *init_lexical(void);
// t_token *init_token(void);
t_simple_cmds *init_cmd(void);
>>>>>>> 9b266628a3f111a29f39f6f4e86aba2cfa59a470
t_lexical *tokenize(char *input);
void process_tokens(t_lexical *tokens);

#endif 