#include "../../includes/minishell.h"

// hadi kat initializer liya gha struct choufi minishell.h bach tfehmi 

t_lexical *init_lexical(void)
{
    t_lexical *lex;
    
    lex = (t_lexical *)malloc(sizeof(t_lexical));
    if (!lex)
        exit(1);
    lex->input = NULL;
    lex->tokens = NULL;
    lex->i = 0;
    return (lex);
}