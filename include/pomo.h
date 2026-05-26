#ifndef POMO_H
#define POMO_H

#include "hw.h"
#include "gui.h"
#include "esp_log.h"
#include "mood.h"

extern bool pomo_running; 
extern uint32_t pomo_time;
String pomo_get_time(uint32_t seconds);
void pomo_start();
void pomo_stop();
void pomo_task(void* param);
void pomo_inc();
void pomo_dec();


// #define BREAK_TEST 1*60
// #define BREAK_SHORT 5*60
// #define BREAK_LONG  10*60

// /* pomodoro timer task note: this task is set to reset by clicking the start button while running */
// void pomo_task(void* param);
// void start_pomo();


#endif