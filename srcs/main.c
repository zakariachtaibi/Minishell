#include "../includes/minishell.h"

int main(int ac, char **av) {
    t_lexical *tokens;
    char *input;

    (void)av;
    if (ac != 1) exit(1);

    while (1) {
        input = readline("minishell> ");
        if (!input) 
            exit(1);
        add_history(input);
        tokens = tokenize(input);

        // while (tokens) 
        // {
        //     printf("Node %d: \n", tokens->i);
        //     printf("  str: %s\n", tokens->str);
        //     printf("  token: %d\n", tokens->token); 
        //     //   printf("  token: %p\n", tokens->next); 
             
        //     if ((tokens->next == NULL) && (tokens->prev == NULL))
        //     {
               
        //         printf("  next : NULL\n");
        //         printf("  prev : NULL\n");
        //     }
        //     else if(tokens->prev == NULL)
        //     {
        //         printf("  next: %s\n", tokens->next->str);
        //         printf("  prev : NULL\n");

        //     }
           
        
        //      else if (tokens->next == NULL)
        //     {
        //         printf("  next : NULL\n");
        //         printf("  prev: %s\n", tokens->prev->str);
        //     } 
        //     else
        //     {
        //         printf("  next: %s\n", tokens->next->str);
        //         printf("  prev: %s\n", tokens->prev->str);
        //     }
        //     tokens = tokens->next;
        // }

        process_tokens(tokens);
        
        // // Free tokens list after processing
        // while (tokens) {
        //     t_lexical *next_token = tokens->next;
        //     free(tokens->str);
        //     free(tokens);
        //     tokens = next_token;
        // }
        free(input);
    }
    // return (0);
};
