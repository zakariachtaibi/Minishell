#include "../includes/minishell.h"


int main()
{
    char *input;

    while (1) 
    {
        input = readline("minishell> ");
        if (!input) {
            break; 
        }
            add_history(input);

        free(input);
    }
    
return(0);

}