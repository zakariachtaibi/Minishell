/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/20 17:03:09 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
            exit(1);
        add_history(input);
        input = validat_input(input);
        if (input == NULL)
            continue;
        tokens = tokenize(input);
        
        process_tokens(tokens);
        free(input);
    }
}
