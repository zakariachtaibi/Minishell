#include "../../includes/minishell.h"

// hadi kat initializer liya gha struct choufi minishell.h bach tfehmi 

t_lexical *init_lexical(void)
{
    t_lexical *lex;
    
    lex = (t_lexical *)malloc(sizeof(t_lexical));
    if (!lex)
        exit(1);
    lex->input = NULL;
    lex->next = NULL;
    lex->prev = NULL;
    lex->i = 0;
    return (lex);
}
// hade fen ka initialiser duk lparams d tokens
void check_input(char *lex, t_token *token)
{
    // t_token *token;
    int i = 0;
    // int j = 0;
    char **splitted;
    splitted  = ft_split(lex, ' ');
    while (*splitted)
    {
        if (strcmp(splitted[i], ">"))
        {
            token->redirect_output = splitted[i];
            i++;
        }
        else if (strcmp(splitted[i], "<"))
        {
            token->redirect_input = splitted[i];
            i++;
        }
        else  if (strcmp(splitted[i], ">>"))
        {
            token->redirect_append = splitted[i];
            i++;
        }
        else  if (strcmp(splitted[i], "<<"))
        {
            token->heredoc = splitted[i];
            i++;
        }
        else  if (strcmp(splitted[i], "|"))
        {
            token->pipe = splitted[i];
            i++;
        }
        else
        {
            token->word = splitted[i];
            i++;
        }

    }
}