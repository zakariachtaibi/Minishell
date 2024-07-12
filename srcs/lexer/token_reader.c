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

t_token *init_token(void)
{
    t_token *token;
    
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        exit(1);
    token->word = malloc(sizeof(char**));
    token->heredoc = NULL;
    token->pipe = NULL;
    token->redirect_output = NULL;
    token->redirect_input = NULL;
    token->redirect_append = NULL;
    return (token);
}
// hade fen ka initialiser duk lparams d tokens
void check_input(char *lex, t_token *token)
{
    int i = 0;
    int j = 0;
    // char *tmp;
    char **splitted;
    // int len = 0;
    
    splitted = ft_split(lex, ' ');
    while (splitted[i])
    {
        if (!ft_strncmp(splitted[i], ">", 2))
        {
            i++;
            token->redirect_output = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "<", 2))
        {
            i++;
            token->redirect_input = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], ">>", 3))
        {
            i++;
            token->redirect_append = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "<<", 3))
        {
            i++;
            token->heredoc = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "|", 2))
        {
            token->pipe = splitted[i];
            // i++;
        }
            
        else
        {
            // if (!token->word)
            // printf("%s", token->word[0]);
            // exit(0);
                token->word[j++] = splitted[i];
                // j++;

            // else
            // {
            //     len = ft_strlen(token->word) + ft_strlen(splitted[i]) + 2;
            //     tmp = malloc(len);
            //     if (!tmp)
            //         exit(1);
            //     ft_strlcpy(tmp, token->word, len);
            //     ft_strlcat(tmp, " ", len);
            //     ft_strlcat(tmp, splitted[i], len);
                // free(token->word);
            //     token->word = tmp;
            // }
            // i++;
        }
        i++;
    }
    free(splitted);
}