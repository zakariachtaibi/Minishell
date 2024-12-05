/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 21:24:50 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/12/05 00:56:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_sigint_child(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	exit(130);
}

void	handle_sigquit_child(int test)
{
	(void)test;
	ft_putstr_fd("Quit (core dumped)\n", 2);
	exit(131);
}

void	setup_child_signals(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, handle_sigquit_child);
}
