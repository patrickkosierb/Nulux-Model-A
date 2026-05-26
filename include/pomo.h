#ifndef POMO_H
#define POMO_H

#include "hw.h"
#include "gui.h"
#include "esp_log.h"
#include "mood.h"

extern bool pomo_running; 
extern uint32_t pomo_time;

String pomo_get_time(uint32_t seconds);

void pomo_task(void* param);
void pomo_start();
void pomo_stop();
void pomo_inc();
void pomo_dec();

#endif