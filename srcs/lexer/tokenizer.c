#include "../../includes/minishell.h"

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
    for (i = 0; word[i]; i++)
        free(word[i]);
    free(word);

    return head;
}
