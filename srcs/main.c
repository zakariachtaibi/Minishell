#include "../includes/minishell.h"

int main(int ac, char **av) 
{
    t_lexical *tokens;
    char *input;

    (void)av;
    if (ac != 1) exit(1);

    while (1) {
        input = readline("minishell> ");
        if (!input) 
            exit(1);
        if (!has_unclosed_quotes(input))
        {
            printf("Error: Unclosed quotes");
            continue ;
        }
        add_history(input);
        tokens = tokenize(input);
        process_tokens(tokens);
        // cleanup(&tokens);
        free(input);
    }
}
