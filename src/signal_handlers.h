#ifndef CADEL__SIGNAL_HANDLERS_H
#define CADEL__SIGNAL_HANDLERS_H

#include <stdbool.h>

void cadel_stop();
bool cadel_running();
bool cadel_register_signal_handlers();

#endif
