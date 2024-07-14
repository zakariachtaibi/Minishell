#ifndef MINISHELL_H

#define MINISHELL_H

// ha hiya hna 
//qaddir token struct hit lexer ghade ghe yqraa l input u ychuf wash word ula token chuf dek roadmap li seft
// typedef struct s_token
// {
//     // char **word ;
//     char *redirect_input;
//     char *redirect_output;
//     char *heredoc;
//     char *redirect_append;
//     char *pipe;
// }   t_token;

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

// t_lexical *init_lexical(void);
// t_token *init_token(void);
t_lexical *tokenize(char *input);

#endif 