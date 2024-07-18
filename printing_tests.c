// printf("before--------------------------------------------\n");
    // while (tokens) 
    // {
    //     printf("Node %d: \n", tokens->i);
    //     printf("  str: %s\n", tokens->str);
    //     printf("  token: %d\n", tokens->token); 

    //     if ((tokens->next == NULL) && (tokens->prev == NULL))
    //     {
    //         printf("  next: NULL\n");
    //         printf("  prev: NULL\n");
    //     }
    //     else if (tokens->prev == NULL)
    //     {
    //         printf("  next: %s\n", tokens->next->str);
    //         printf("  prev: NULL\n");
    //     }
    //     else if (tokens->next == NULL)
    //     {
    //         printf("  next: NULL\n");
    //         printf("  prev: %s\n", tokens->prev->str);
    //     } 
    //     else
    //     {
    //         printf("  next: %s\n", tokens->next->str);
    //         printf("  prev: %s\n", tokens->prev->str);
    //     }
    //     tokens = tokens->next;
    // }
     // Test
    // printf("after---------------------------------------------\n");
    // while (temp_tokens) 
    // {
    //     printf("Node %d: \n", temp_tokens->i);
    //     printf("  str: %s\n", temp_tokens->str);
    //     printf("  token: %d\n", temp_tokens->token); 

    //     if ((temp_tokens->next == NULL) && (temp_tokens->prev == NULL))
    //     {
    //         printf("  next: NULL\n");
    //         printf("  prev: NULL\n");
    //     }
    //     else if (temp_tokens->prev == NULL)
    //     {
    //         printf("  next: %s\n", temp_tokens->next->str);
    //         printf("  prev: NULL\n");
    //     }
    //     else if (temp_tokens->next == NULL)
    //     {
    //         printf("  next: NULL\n");
    //         printf("  prev: %s\n", temp_tokens->prev->str);
    //     } 
    //     else
    //     {
    //         printf("  next: %s\n", temp_tokens->next->str);
    //         printf("  prev: %s\n", temp_tokens->prev->str);
    //     }
    //     temp_tokens = temp_tokens->next;
    // }

    // t_simple_cmds *cmd_temp = cmds_head;
    // while (cmd_temp) 
    // {
    //     if (cmd_temp->str) 
    //     {
    //         int i = 0;
    //         while (cmd_temp->str[i]) 
    //         {
    //             printf("command [%d]: %s\n", i, cmd_temp->str[i]);
    //             i++;
    //         }
    //     } 
    //     else 
    //     {
    //         printf("(null)");
    //     }
    //     printf("\nRedirections: ");
    //     if (cmd_temp->redirections) 
    //     {
    //         t_lexical *redir_temp = cmd_temp->redirections;
    //         while (redir_temp) 
    //         {
    //             printf("%s ", redir_temp->str);
    //             redir_temp = redir_temp->next;
    //         }
    //     } 
    //     else 
    //     {
    //         printf("(null)");
    //     }
    //     if (cmd_temp->hd_file_name) 
    //     {
    //         printf("\nHeredoc delimiter: %s", cmd_temp->hd_file_name);
    //     } 
    //     else 
    //     {
    //         printf("\nHeredoc delimiter: (null)");
    //     }
    //     printf("\n\n");
    //     printf("Num of redire: %d\n",cmd_temp->num_redirections);
    //     cmd_temp = cmd_temp->next;

    // }