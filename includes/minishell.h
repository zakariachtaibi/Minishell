#ifndef MINISHELL_H

#define MINISHELL_H


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



# include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

t_lexical *tokenize(char *input);

#endif 