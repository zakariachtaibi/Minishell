/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchtaibi <zchtaibi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 16:36:08 by zchtaibi          #+#    #+#             */
/*   Updated: 2024/11/10 21:35:42 by zchtaibi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void sig_handler1(int test)
{
    (void)test;
    printf("Quit (core dumped)\n");
}

void sigint2(int sig)
{
    (void)sig;
    write(1, "\n", 1);
}
