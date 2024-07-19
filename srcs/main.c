/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/19 12:23:48 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av)
{
	t_lexical	*tokens;
	char		*input;

	(void)av;
	if (ac != 1)
		exit(1);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			exit(1);
		add_history(input);
		check_input(input);
		tokens = tokenize(input);
		while (tokens) 
		{
			printf("  str: %s\n", tokens->str);
			tokens = tokens->next;
		}
		process_tokens(tokens);
		free(input);
	}
}
