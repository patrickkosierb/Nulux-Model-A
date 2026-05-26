#ifndef STATE_H
#define STATES_H

#include "hw.h"
#include "gui.h"
#include "mood.h"
#include "esp_log.h"
#include "habit.h"
#include "pomo.h"

#define STATE_MENU 0
#define STATE_POMO 1 

extern bool screen_state;

/* state machine for device */
void state_machine();

#endif
