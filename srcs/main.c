#include "../includes/minishell.h"


int main()
{
    char *input;

    while (1) 
    {
        // Display the prompt and read input
        input = readline("minishell> ");
        if (!input) {
            break; 
        }
            add_history(input);
        // printf("You entered: %s\n", input);

        free(input);
    }
    
return(0);

}