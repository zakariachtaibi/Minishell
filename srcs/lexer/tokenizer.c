#include "../../includes/minishell.h"

t_lexical *tokenize(char *input)
{
    char **word;
    t_lexical *head = NULL;
    t_lexical *current = NULL;
    t_lexical *node;
    int i;
    i = 0;
    
    word = ft_split(input, ' ');

    while (word[i])
    {
        node = malloc(sizeof(t_lexical));
        node->str = ft_strdup(word);
        node->i = i;
        if (word == "|")
            node->token = TOKEN_PIPE;
        else if (word == "<")
            node->token = TOKEN_REDIRECT_IN;
        else if (word == "<<")
            node->token = TOKEN_HEREDOC;
        else if (word == ">")
            node->token = TOKEN_REDIRECT_OUT;
        else if (word == ">>")
            node->token = TOKEN_APPEND;
        else
            node->token = TOKEN_WORD;
    }
    
}