#ifndef HABIT_H
#define HABIT_H

#include <Preferences.h>
#include <time.h>

#define NUM_HABITS 4

struct Habit{
    const char* name; 
    bool is_done;    
    int streak;
};

extern Habit habits[NUM_HABITS];

uint16_t habits_avg_streak(void);
uint8_t habits_completed(Habit * habit);

// NVS persistence
void load_state();
void save_state();
void load_timestamp();
void save_timestamp();

// time related
void set_time_utc(int year, int month, int day, int hour, int min, int sec);
bool time_is_valid();
void check_day_rollover();

#endif