// mood.cpp
#include "mood.h"
#include "habit.h"

#include <math.h>

static MoodState s_current   = MOOD_NEUTRAL;
static MoodState s_candidate = MOOD_NEUTRAL;
static uint8_t   s_ticks     = 0;

static const uint8_t HYSTERESIS_TICKS = 3;

static float completion_score(uint8_t done) {
  if (NUM_HABITS == 0) return 0.0f;
  return (float)done / NUM_HABITS;
}

static float streak_score(uint16_t streak) {
  return log((float)streak + 1.0f) / log(31.0f);
}

static float time_pressure(uint8_t hour, float completion) {
  if (completion >= 1.0f || hour < 19) return 0.0f;
  float p = (float)(hour - 19) / 3.5f;
  return p > 1.0f ? 1.0f : p;
}

static MoodState score_to_state(uint8_t done, uint16_t streak, uint8_t hour) {
  if (hour >= 23 || hour < 6)              return MOOD_SLEEPING;
  if (NUM_HABITS == 0)                          return MOOD_NEUTRAL;

  float C = completion_score(done);
  float S = streak_score(streak);
  float P = time_pressure(hour, C);
  float w = 0.5f * C + 0.3f * S - 0.2f * P;

  if (hour >= 6 && hour < 9 && C < 0.1f)  return MOOD_NEUTRAL;
  if (C >= 1.0f && streak >= 7)            return MOOD_THRIVING;
  if (C >= 1.0f && streak >= 3)            return MOOD_HAPPY;      // <-- add
  if (C >= 1.0f)                           return MOOD_SATISFIED;
  if (w >= 0.65f)                          return MOOD_HAPPY;
  if (P > 0.5f && C < 0.7f)               return MOOD_WORRIED;
  return MOOD_NEUTRAL;
}

MoodState mood_evaluate(uint8_t habits_done, uint16_t streak, uint8_t hour) {

  // if (habits_done==NUM_HABITS)  return s_current = MOOD_CELEBRATING;

  MoodState scored = score_to_state(habits_done, streak, hour);
                          
  if (scored != s_current) {
    if (scored == s_candidate) {
      if (++s_ticks >= HYSTERESIS_TICKS) {
        s_current = s_candidate;
        s_ticks   = 0;
      }
    } else {
      s_candidate = scored;
      s_ticks     = 1;
    }
  } else {
    s_candidate = s_current;
    s_ticks     = 0;
  }

  return s_current;
}