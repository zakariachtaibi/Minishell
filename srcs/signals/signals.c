#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;

    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    return ;
}


void sig_handler1(int test)
{
    (void)test;
    printf("Quit (core dumped)\n");
    return ;
}

void sigint2()
{
	write(1, "\n", 1);
}