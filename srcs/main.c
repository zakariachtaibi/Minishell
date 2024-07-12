#include "../includes/minishell.h"

int main(int ac, char **av)
{
    t_lexical *lex;
    t_token *token;
    (void)av;
token = NULL;
    if (ac != 1)
        exit(1);
    lex = init_lexical();
    while (1) {
        lex->input = readline("minishell> ");
        if (!lex->input)
            break;
        add_history(lex->input);
        // hna khedmt b split
        // lex->i = 0;
        // hna kaneayet liha bach teamrhuum
        check_input(lex->input, token);
        // hna rah ghi kan afficher tokens kanchouf wach hadchi khedam
        // while (lex->tokens[lex->i])
        // {
        //     printf("test token %d: %s\n", lex->i, lex->tokens[lex->i]);
        //     free(lex->tokens[lex->i]);
        //     lex->i++;
        // }
// u hna bghet njreb nprinter nchuf
        printf("test token : %s\n", token->word);
         printf("test token : %s\n", token->heredoc);
          printf("test token : %s\n", token->pipe);
    }
    free(lex);
    return (0);
}
