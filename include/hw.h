#ifndef HW_H
#define HW_H

#include <Arduino.h>

#define ONBOARD_LED     GPIO_NUM_2
#define LIFE_LED        GPIO_NUM_22
#define GO_LED          GPIO_NUM_21
#define RIGHT_BUTTON    GPIO_NUM_14
#define LEFT_BUTTON   GPIO_NUM_13
#define MID_BUTTON    GPIO_NUM_12
#define BUZZER          GPIO_NUM_15

/* button states */
#define BUTTON_DEFUALT -1
#define RIGHT_PRESSED   0
#define MID_PRESSED  1
#define LEFT_PRESSED   2
#define MID_HELD   3

/* initialize GPIO */
void init_hw(void);
/* top button ISR */
void IRAM_ATTR rHandleISR();
/* middle button ISR */
void IRAM_ATTR midHandleISR();
/* bottom button ISR */
void IRAM_ATTR lHandleISR();
/* life indicator task */
void life_task(void * parameter);
void start_life();
/* logic for button press */
bool pressed();
/* buzzer activate task */
void buzz();
void buzz_bling();
void buzz_victory();
void buzz_startup();
void buzz_pomo_start();
void buzz_pomo_break();
void buzz_pomo_resume();

#endif