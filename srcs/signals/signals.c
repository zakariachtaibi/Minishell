#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;

    write(1, "\n", 1);
    write(1, "minishell> ", 11);
}
void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = &handle_sigint;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
