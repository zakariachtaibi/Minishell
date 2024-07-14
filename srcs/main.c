#include "../includes/minishell.h"

int main(int ac, char **av)
{
    t_lexical *lex;
    t_token *token;

    (void)av;
    if (ac != 1)
        exit(1);

    lex = init_lexical();
    token = init_token();
    while (1)
    {
        lex->input = readline("minishell> ");
        if (!lex->input)
            break;
        add_history(lex->input);
        check_input(lex->input, token);

        int i = 0;
        while (i < 4)
        {
            printf("word[%d] : %s\n", i, token->word[i]);
            i++;
        }
        printf("cmd : %s\n", token->cmd);
        printf("test token herdoc : %s\n", token->heredoc);
        printf("test token pipe : %s\n", token->pipe);
        printf("test token redirect_output : %s\n", token->redirect_output);
        printf("test token redirect_input : %s\n", token->redirect_input);
        printf("test token redirect_append : %s\n", token->redirect_append);

        free(lex->input);
    }

    free(lex);
    for (int i = 0; token->word[i]; i++) {
        free(token->word[i]);
    }
    free(token->word);
    free(token->cmd); // Free cmd
    free(token);
    return (0);
}

