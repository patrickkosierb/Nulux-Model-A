#ifndef STATE_H
#define STATES_H

#include "hw.h"
#include "gui.h"
#include "mood.h"
#include "esp_log.h"
#include "habit.h"
#include "pomo.h"

// TODO: put this in an emum or struct
#define STATE_MENU 0
#define STATE_POMO 1 

#define SLEEPING 0
#define NEUTRAL 1
#define HAPPY 2
#define SAD 3
#define SUPERHAPPY 4

extern bool screen_state;

/* state machine for device */
void state_machine();

#endif
