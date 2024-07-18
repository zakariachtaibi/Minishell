/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchouai <hchouai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:13:20 by hchouai           #+#    #+#             */
/*   Updated: 2024/07/18 13:16:17 by hchouai          ###   ########.fr       */
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
		if (!has_unclosed_quotes(input))
		{
			printf("Error: Unclosed quotes");
			continue ;
		}
		add_history(input);
		tokens = tokenize(input);
		process_tokens(tokens);
		free(input);
	}
}
