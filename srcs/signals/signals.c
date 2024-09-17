#include "../../includes/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;

    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
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
