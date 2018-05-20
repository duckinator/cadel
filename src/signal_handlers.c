#include "cadel.h"
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

static volatile bool _cadel_running = true;

void cadel_stop()
{
    _cadel_running = false;
}

bool cadel_running()
{
    return _cadel_running;
}

void cadel_interrupt_handler(int signal_number)
{
    cadel_stop();
}

bool cadel_register_signal_handlers()
{
    struct sigaction new_sigaction;
    new_sigaction.sa_handler = cadel_interrupt_handler;

    if (atexit(&cadel_stop) != 0) {
        return false;
    }
    if (sigaction(SIGINT, &new_sigaction, NULL) != 0) {
        return false;
    }

    return true;
}
