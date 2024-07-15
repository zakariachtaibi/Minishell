#include "../../includes/minishell.h"

t_simple_cmds *init_cmd(void)
{
    t_simple_cmds *new_cmd = malloc(sizeof(t_simple_cmds));
    new_cmd->next = NULL;
    new_cmd->prev = NULL;
    new_cmd->str = NULL;
    new_cmd->num_redirections = 0;
    new_cmd->hd_file_name = NULL;
    new_cmd->redirections = NULL;
    return new_cmd;
}

void add_redirection(t_lexical **redirections, t_lexical *redir_node)
{
    redir_node->next = *redirections;
    if (*redirections) (*redirections)->prev = redir_node;
    *redirections = redir_node;
}

void process_tokens(t_lexical *tokens)
{
    t_simple_cmds *cmds_head;
    t_simple_cmds *current_cmd;
    t_simple_cmds *new_cmd;
    t_lexical *temp;

    cmds_head = NULL;
    current_cmd = NULL;
    temp = tokens;
    while (temp)
    {
        if (current_cmd == NULL || temp->token == TOKEN_PIPE)
        {
            new_cmd = init_cmd();
            if (!cmds_head)
                cmds_head = new_cmd;
            else
            {
                current_cmd->next = new_cmd; 
                new_cmd->prev = current_cmd;
            }
            current_cmd = new_cmd;

            if (temp->token == TOKEN_PIPE)
            {
                temp = temp->next;
                continue;
            }
        }

        // redirections
        if (temp->token == TOKEN_REDIRECT_IN || temp->token == TOKEN_REDIRECT_OUT ||
            temp->token == TOKEN_HEREDOC || temp->token == TOKEN_APPEND)
        {
            t_lexical *redir;

            redir = malloc(sizeof(t_lexical));
            *redir = *temp;
            redir->next = current_cmd->redirections;
            if (current_cmd->redirections)
                current_cmd->redirections->prev = redir;
            current_cmd->redirections = redir;
            current_cmd->num_redirections++;

            temp = temp->next;
            if (temp && temp->token == TOKEN_WORD)
            {
                current_cmd->hd_file_name = strdup(temp->str);
                temp = temp->next;
            }
            continue;
        }

        // words
        if (temp->token == TOKEN_WORD)
        {
            int word_count = 0;
            t_lexical *word_temp = temp;
            while (word_temp && word_temp->token == TOKEN_WORD)
            {
                word_count++;
                word_temp = word_temp->next;
            }

            current_cmd->str = malloc(sizeof(char *) * (word_count + 1));
            int i = 0;
            while (temp && temp->token == TOKEN_WORD)
            {
                current_cmd->str[i] = strdup(temp->str);
                temp = temp->next;
                i++;
            }
            current_cmd->str[i] = NULL;
            continue;
        }

        temp = temp->next;
    }

    // Test
    t_simple_cmds *cmd_temp = cmds_head;
    while (cmd_temp)
    {
        printf("Command: ");
        if (cmd_temp->str)
        {
            int i;
            i = -1;
            while (cmd_temp->str[++i])
                printf("%s ", cmd_temp->str[i]);
        }
        if (cmd_temp->redirections)
        {
            printf("\nRedirections: ");
            t_lexical *redir_temp = cmd_temp->redirections;
            while (redir_temp)
            {
            printf("%s\n", redir_temp->str);
            redir_temp = redir_temp->next;
            }
        }
        if (cmd_temp->hd_file_name)
            printf("Heredoc file: %s\n", cmd_temp->hd_file_name);
        printf("\n");
        cmd_temp = cmd_temp->next;
    }
}