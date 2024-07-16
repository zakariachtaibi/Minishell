#include "../../includes/minishell.h"

int has_unclosed_quotes(char *temp)
{
    char *input;
    input = temp;
    int i = 0;
    int c = 0;
    while(input[i])
    {
        if (input[i] == '"')
            c++;
        i++;
    }
    if (c % 2 == 0)
        return(1);
    else
        return(0);
}

t_lexical *tokenize(char *input)
{
    char **word;
    t_lexical *head = NULL;
    t_lexical *current = NULL;
    t_lexical *node;
    int i;

    word = ft_split(input, ' ');
    if (!word)
        return NULL;

    i = 0;
    while (word[i])
    {
        node = malloc(sizeof(t_lexical));
        if (!node)
            exit(1);
        node->str = ft_strdup(word[i]);
        if (!node->str)
            exit(1);
        node->i = i;
        if (strcmp(word[i], "|") == 0)
            node->token = TOKEN_PIPE;
        else if (strcmp(word[i], "<") == 0)
            node->token = TOKEN_REDIRECT_IN;
        else if (strcmp(word[i], "<<") == 0)
            node->token = TOKEN_HEREDOC;
        else if (strcmp(word[i], ">") == 0)
            node->token = TOKEN_REDIRECT_OUT;
        else if (strcmp(word[i], ">>") == 0)
            node->token = TOKEN_APPEND;
        else
            node->token = TOKEN_WORD;

        node->next = NULL;
        node->prev = current;

        if (current)
            current->next = node;
        else
            head = node;

        current = node;
        i++;
    }
    free(word);
    return(head);
}
