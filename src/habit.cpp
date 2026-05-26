#include "habit.h"
#include "state.h"
#include "mood.h"

Habit habits[] = {
    {"read",     false, 0},
    {"meditate", false, 0},
    {"exercise",      false, 0},
    {"floss",      false, 0},
};


static Preferences prefs;

int check_state(Habit * habit){
    int total_done=0;

    for(int i=0;i<NUM_HABITS;i++){
        if(habit[i].is_done)
            total_done++;
    }   

    if(total_done==(NUM_HABITS)){
        return SUPERHAPPY;
    }else if(total_done>NUM_HABITS/2){
        return HAPPY;
    }
    else if(total_done==0){
        return SAD;
    }else{
        return NEUTRAL;
    }
}

uint16_t habits_avg_streak() {
  if (NUM_HABITS == 0) return 0;
  uint32_t sum = 0;
  for (int i = 0; i < NUM_HABITS; i++) sum += habits[i].streak;
  return sum / NUM_HABITS;
}

uint8_t habits_completed(Habit * habit){
    int total_done=0;
    for(int i=0;i<NUM_HABITS;i++){
        if(habit[i].is_done)
            total_done++;
    }   
    return total_done;
}

void load_state() {
    prefs.begin("habiko", false);
    for (int i = 0; i < 4; i++) {
        char done_key[4], streak_key[4];
        snprintf(done_key,   sizeof(done_key),   "h%d", i);
        snprintf(streak_key, sizeof(streak_key), "s%d", i);
        habits[i].is_done = prefs.getBool(done_key,   false);
        habits[i].streak  = prefs.getInt (streak_key, 0);
    }
    prefs.end();
}

void save_state() {
    prefs.begin("habiko", false);
    for (int i = 0; i < 4; i++) {
        char done_key[4], streak_key[4];
        snprintf(done_key,   sizeof(done_key),   "h%d", i);
        snprintf(streak_key, sizeof(streak_key), "s%d", i);
        prefs.putBool(done_key,   habits[i].is_done);
        prefs.putInt (streak_key, habits[i].streak);
    }
    prefs.end();
}

void save_timestamp() {
    time_t now = time(nullptr);
    prefs.begin("habiko", false);
    prefs.putLong("timestamp", (long)now);
    prefs.end();
}

void load_timestamp() {
    prefs.begin("habiko", false);
    long saved = prefs.getLong("timestamp", 0);
    prefs.end();

    if (saved > 1000000000L) {
        struct timeval tv = { .tv_sec = (time_t)saved };
        settimeofday(&tv, nullptr);
    }
}

// time related
void set_time_utc(int year, int month, int day, int hour, int min, int sec) {
    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min  = min;
    t.tm_sec  = sec;
    time_t epoch = mktime(&t);
    struct timeval tv = { .tv_sec = epoch };
    settimeofday(&tv, nullptr);
}

bool time_is_valid() {
    return time(nullptr) > 1000000000UL;
}

void check_day_rollover() {
    if (!time_is_valid())
        return;  // no valid time, skip — don't punish user 

    bool any_broke = false;
    for (int i = 0; i < 4; i++) {
        if (!habits[i].is_done && habits[i].streak > 0) {
            any_broke = true;
        }
        habits[i].streak  = habits[i].is_done ? habits[i].streak : 0 ; 
        habits[i].is_done = false;
    }
    
    save_state();
    if(any_broke)
        update_penguin(MOOD_SAD);
    else
        update_penguin(check_state(habits));
    
    draw_habit(0);
}