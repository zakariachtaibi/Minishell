#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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
    }
return(0);

}