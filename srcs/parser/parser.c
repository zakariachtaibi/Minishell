#include "../../includes/minishell.h"

t_simple_cmds *init_cmd(void)
{
    t_simple_cmds *new_cmd = malloc(sizeof(t_simple_cmds));
    new_cmd->next = NULL;
    new_cmd->prev = NULL;
    new_cmd->str = NULL;
    new_cmd->num_redirections = 0;
    new_cmd->hd_file_name = NULL;
    return (new_cmd);
}

void process_tokens(t_lexical *tokens)
{
    t_simple_cmds *cmds = NULL;
    t_simple_cmds *current_cmd = NULL;

    while (tokens)
    {
        t_simple_cmds *new_cmd;

        new_cmd = init_cmd();
        t_lexical *start = tokens;
        while (tokens && tokens->token != TOKEN_PIPE)
            tokens = tokens->next;

        int len = 0;
        t_lexical *temp = start;
        
        while (temp && temp != tokens)
        {
            len++;
            temp = temp->next;
        }

        new_cmd->str = malloc(sizeof(char *) * (len + 1));

        int i = -1;
        while (++i < len)
        {
            new_cmd->str[i] = ft_strdup(start->str);
            start = start->next;
        }
        new_cmd->str[len] = NULL;

        if (!cmds)
            cmds = new_cmd;
        else 
        {
            current_cmd->next = new_cmd;
            new_cmd->prev = current_cmd;
        }
        current_cmd = new_cmd;

        if (tokens && tokens->token == TOKEN_PIPE)
            tokens = tokens->next;
    }

    current_cmd = cmds;
    int num = 0;
    while (current_cmd)
    {
        printf("----------Command[%d]---------: ", num);
        int i = -1;
        while (current_cmd->str[++i])
            printf("%s ", current_cmd->str[i]);
        printf("\n");
        current_cmd = current_cmd->next;
        num++;
    }
}