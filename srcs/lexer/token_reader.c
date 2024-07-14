#include "../../includes/minishell.h"

// hadi kat initializer liya gha struct choufi minishell.h bach tfehmi 

t_lexical *init_lexical(void)
{
    t_lexical *lex;
    
    lex = (t_lexical *)malloc(sizeof(t_lexical));
    if (!lex)
        exit(1);
    lex->input = NULL;
    lex->next = NULL;
    lex->prev = NULL;
    lex->i = 0;
    return (lex);
}

t_token *init_token(void)
{
    t_token *token;
    int     i;
    
    token = (t_token *)malloc(sizeof(t_token));
    if (!token)
        exit(1);
    token->word = malloc(sizeof(char*) * 10);
    token->cmd = malloc(sizeof(char*) * 10);
    if (!token->cmd)
        exit(1);
    if (!token->word)
        exit(1);
    i = -1;
    while (++i < 10)
    {
        token->word[i] = NULL;
        token->cmd[i] = NULL;
    }
    token->heredoc = NULL;
    token->pipe = NULL;
    token->redirect_output = NULL;
    token->redirect_input = NULL;
    token->redirect_append = NULL;
    return (token);
}

void check_input(char *lex, t_token *token)
{
    int i = 0;
    int j = 0;
    char **splitted;
    char *cmd;
    size_t cmd_len = 0;
    
    splitted = ft_split(lex, ' ');
    while (splitted[i])
    {
        if (!ft_strncmp(splitted[i], ">", 2))
        {
            i++;
            token->redirect_output = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "<", 2))
        {
            i++;
            token->redirect_input = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], ">>", 3))
        {
            i++;
            token->redirect_append = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "<<", 3))
        {
            i++;
            token->heredoc = splitted[i];
        }
        else if (!ft_strncmp(splitted[i], "|", 2))
        {
            token->pipe = splitted[i];
            i++;
            continue;
        }
        else
        {
            token->word[j++] = splitted[i];
        }
        i++;
    }
    
    cmd_len = 0;
    i = 0;
    while (token->word[i])
    {
        cmd_len += strlen(token->word[i]) + 1;
        i++;
    }
    cmd = malloc(cmd_len + 1);
    if (!cmd)
        exit(1);
    cmd[0] = '\0';
    for (i = 0; token->word[i]; i++)
    {
        ft_strlcat(cmd, token->word[i], ft_strlen(token->word[i]));
        if (token->word[i + 1])
            strcat(cmd, " ");
    }
    
    token->cmd = cmd;
    
    free(splitted);
}
