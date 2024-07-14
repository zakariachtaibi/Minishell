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
        // process_tokens(tokens);
        }
    return (0);
}
