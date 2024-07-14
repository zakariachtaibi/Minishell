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
     node = malloc(sizeof(t_lexical));
     node->prev = NULL;
    while (word[i])
    {
        node = malloc(sizeof(t_lexical));
        node->str = ft_strdup(word[i]);
        node->i = i;
        if (strcmp(word[i],"|") == 0)
            node->token = TOKEN_PIPE;
        else if (strcmp(word[i],"<") == 0)
            node->token = TOKEN_REDIRECT_IN;
        else if (strcmp(word[i],"<<") == 0)
            node->token = TOKEN_HEREDOC;
        else if (strcmp(word[i],">") == 0)
            node->token = TOKEN_REDIRECT_OUT;
        else if (strcmp(word[i],">>") == 0)
            node->token = TOKEN_APPEND;
        else
            node->token = TOKEN_WORD;

        if (head == NULL)
        {
            // node->prev = NULL;
            head = node;
            current = node;
        }
        else
        {
            current->next = node;
            node->prev = current;
            current = node;
        }
        i++;
    }
    free(word);
    return(head);
}