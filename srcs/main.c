#include "../includes/minishell.h"

// hadi kat initializer liya gha struct choufi minishell.h bach tfehmi 

t_lexical *init_lexical(void)
{
    t_lexical *lex;
    
    lex = (t_lexical *)malloc(sizeof(t_lexical));
    if (!lex)
        exit(1);
    lex->input = NULL;
    lex->tokens = NULL;
    lex->i = 0;
    return (lex);
}

int main(int ac, char **av)
{
    t_lexical *lex;
    (void)av;

    if (ac != 1)
        exit(1);
    lex = init_lexical();
    while (1) {
        lex->input = readline("minishell> ");
        if (!lex->input)
            break;
        add_history(lex->input);
        // hna khedmt b split
        lex->tokens = ft_split(lex->input, ' ');
        lex->i = 0;
        // hna rah ghi kan afficher tokens kanchouf wach hadchi khedam
        while (lex->tokens[lex->i])
        {
            printf("test token %d: %s\n", lex->i, lex->tokens[lex->i]);
            free(lex->tokens[lex->i]);
            lex->i++;
        }
        free(lex->tokens);
        free(lex->input);
    }
    free(lex);
    return (0);
}
