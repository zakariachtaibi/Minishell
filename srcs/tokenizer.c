#include "../includes/minishell.h"

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
    }
}