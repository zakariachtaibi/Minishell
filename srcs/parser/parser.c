#include "../../includes/minishell.h"

void add_redirection(t_lexical **redirections, t_lexical *redir_node)
{
    if (*redirections == NULL) 
    {
        // If the list is empty, make redir_node the first node
        *redirections = redir_node;
    } 
    else 
    {
        // Traverse to the end of the list
        t_lexical *current = *redirections;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        // Add redir_node at the end
        current->next = redir_node;
        redir_node->prev = current;
    }
    redir_node->next = NULL; // Make sure the new node is the last node
}

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

t_lexical *copy_node(t_lexical *src) {
    if (src == NULL) return NULL;

    // Allocate memory for the new node
    t_lexical *dest = (t_lexical *)malloc(sizeof(t_lexical));
    if (dest == NULL) return NULL;

    // Copy the simple fields
    dest->token = src->token;
    dest->i = src->i;
    dest->next = NULL; // Initialize next to NULL (or handle linking separately)
    dest->prev = NULL; // Initialize prev to NULL (or handle linking separately)

    // Allocate memory for the string and copy it
    if (src->str != NULL) {
        dest->str = strdup(src->str);
        if (dest->str == NULL) {
            // Handle memory allocation failure
            free(dest);
            return NULL;
        }
    } else {
        dest->str = NULL;
    }

    return dest;
}

void delete_node(t_lexical **head, t_lexical *node_to_delete) 
{
    // If the list is empty or node_to_delete is NULL, do nothing
    if (node_to_delete == NULL) 
    {
        return;
    }

    // Update head if necessary
    if (*head == node_to_delete)
    {
        *head = node_to_delete->next;
    }

    // Change next only if node_to_delete is NOT the last node
    if (node_to_delete->next != NULL) {
        node_to_delete->next->prev = node_to_delete->prev;
    }

    // Change prev only if node_to_delete is NOT the first node
    if (node_to_delete->prev != NULL) {
        node_to_delete->prev->next = node_to_delete->next;
    }

    // Free the memory allocated for node_to_delete
    free(node_to_delete->str);
    free(node_to_delete);
}

void process_tokens(t_lexical *tokens)
{
    t_lexical *temp_tokens;
    t_simple_cmds *cmds_head;
    t_simple_cmds *current_cmd;
    t_simple_cmds *new_cmd;
    t_lexical *temp;

    cmds_head = NULL;
    current_cmd = NULL;
    temp = tokens;
    temp_tokens = tokens;

    printf("before--------------------------------------------\n");
    while (tokens) 
    {
        printf("Node %d: \n", tokens->i);
        printf("  str: %s\n", tokens->str);
        printf("  token: %d\n", tokens->token); 

        if ((tokens->next == NULL) && (tokens->prev == NULL))
        {
            printf("  next: NULL\n");
            printf("  prev: NULL\n");
        }
        else if (tokens->prev == NULL)
        {
            printf("  next: %s\n", tokens->next->str);
            printf("  prev: NULL\n");
        }
        else if (tokens->next == NULL)
        {
            printf("  next: NULL\n");
            printf("  prev: %s\n", tokens->prev->str);
        } 
        else
        {
            printf("  next: %s\n", tokens->next->str);
            printf("  prev: %s\n", tokens->prev->str);
        }
        tokens = tokens->next;
    }

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

        // Handle redirections
        if (temp->token == TOKEN_REDIRECT_IN || temp->token == TOKEN_REDIRECT_OUT ||
            temp->token == TOKEN_HEREDOC || temp->token == TOKEN_APPEND)
        {
            t_lexical *redir = copy_node(temp);
            add_redirection(&current_cmd->redirections, redir);
            current_cmd->num_redirections++;
            delete_node(&tokens, temp);
            temp = temp->next;
            if (temp && temp->token == TOKEN_WORD)
            {
                if (redir->token == TOKEN_HEREDOC)
                {
                    current_cmd->hd_file_name = strdup(temp->str);
                     delete_node(&tokens, temp);
                }
                else
                {
                    t_lexical *filename = copy_node(temp);
                    add_redirection(&current_cmd->redirections, filename);
                      delete_node(&tokens, temp);
                }
                temp = temp->next;
            }
            continue;
        }

        // Handle words
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
    printf("after---------------------------------------------\n");
    while (temp_tokens) 
    {
        printf("Node %d: \n", temp_tokens->i);
        printf("  str: %s\n", temp_tokens->str);
        printf("  token: %d\n", temp_tokens->token); 

        if ((temp_tokens->next == NULL) && (temp_tokens->prev == NULL))
        {
            printf("  next: NULL\n");
            printf("  prev: NULL\n");
        }
        else if (temp_tokens->prev == NULL)
        {
            printf("  next: %s\n", temp_tokens->next->str);
            printf("  prev: NULL\n");
        }
        else if (temp_tokens->next == NULL)
        {
            printf("  next: NULL\n");
            printf("  prev: %s\n", temp_tokens->prev->str);
        } 
        else
        {
            printf("  next: %s\n", temp_tokens->next->str);
            printf("  prev: %s\n", temp_tokens->prev->str);
        }
        temp_tokens = temp_tokens->next;
    }

    t_simple_cmds *cmd_temp = cmds_head;
    while (cmd_temp) 
    {
        if (cmd_temp->str) 
        {
            int i = 0;
            while (cmd_temp->str[i]) 
            {
                printf("command [%d]: %s\n", i, cmd_temp->str[i]);
                i++;
            }
        } 
        else 
        {
            printf("(null)");
        }
        printf("\nRedirections: ");
        if (cmd_temp->redirections) 
        {
            t_lexical *redir_temp = cmd_temp->redirections;
            while (redir_temp) 
            {
                printf("%s ", redir_temp->str);
                redir_temp = redir_temp->next;
            }
        } 
        else 
        {
            printf("(null)");
        }
        if (cmd_temp->hd_file_name) 
        {
            printf("\nHeredoc delimiter: %s", cmd_temp->hd_file_name);
        } 
        else 
        {
            printf("\nHeredoc delimiter: (null)");
        }
        printf("\n\n");
        printf("Num of redire: %d\n",cmd_temp->num_redirections);
        cmd_temp = cmd_temp->next;

    }
}
