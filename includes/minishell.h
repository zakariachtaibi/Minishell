#ifndef MINISHELL_H

#define MINISHELL_H

// ha hiya hna 

typedef struct s_lexical
{
    char *input;
    char **tokens;
    int i;
} t_lexical;


# include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#endif 