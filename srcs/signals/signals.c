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
