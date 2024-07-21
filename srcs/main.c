/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/21 11:58:28 by hchouai          ###   ########.fr       */
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
        tokens = validate_syntax(tokens);
        if (tokens == NULL)
            continue;
        process_tokens(tokens);
        free(input);
    }
}
