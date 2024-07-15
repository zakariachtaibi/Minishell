#include "../includes/minishell.h"

int main(int ac, char **av)
{
    t_lexical *tokens;
    char *input;

    (void)av;
    if (ac != 1)
        exit(1);

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        add_history(input);
        tokens = tokenize(input);
        // while (tokens != NULL) 
        // {
        //     printf("Node %d: \n", tokens->i);
        //     printf("  str: %s\n", tokens->str);
        //     printf("  token: %d\n", tokens->token);
            
        //     if(tokens->prev == NULL)
        //     {
        //         printf("  next: %s\n", tokens->next->str);
        //         printf("  prev : NULL\n");

        //     }
        //     else if (tokens->next == NULL)
        //     {
        //         printf("  next : NULL\n");
        //         printf("  prev: %s\n", tokens->prev->str);
        //     } else
        //     {
        //         printf("  next: %s\n", tokens->next->str);
        //         printf("  prev: %s\n", tokens->prev->str);
        //     }
        //     tokens = tokens->next;
        // }
        process_tokens(tokens);
    }
    // return (0);
};