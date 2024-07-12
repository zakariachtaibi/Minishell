#include "../includes/minishell.h"

int main(int ac, char **av)
{
    t_lexical *lex;
    t_token *token;
    int i = 0;

    (void)av;
    if (ac != 1)
        exit(1);

    lex = init_lexical();
    while (1)
    {
        token = init_token();
        lex->input = readline("minishell> ");
        if (!lex->input)
            break;
        add_history(lex->input);
        check_input(lex->input, token);

        printf("test token  word[%d] : %s\n", i, token->word);
        i++;
        printf("test token herdoc : %s\n", token->heredoc);
        printf("test token pipe : %s\n", token->pipe);
        printf("test token redirect_output : %s\n", token->redirect_output);
        printf("test token redirect_input : %s\n", token->redirect_input);
        printf("test token redirect_append : %s\n", token->redirect_append);

        free(lex->input);
    }

    free(lex);
    free(token);
    return (0);
}
