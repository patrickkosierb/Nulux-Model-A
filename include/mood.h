// mood.h
#pragma once
#include <stdint.h>

enum MoodState {
  MOOD_SLEEPING,//
  MOOD_NEUTRAL,//
  MOOD_WORRIED,//
  MOOD_SATISFIED,
  MOOD_HAPPY,//
  MOOD_THRIVING,//
  MOOD_CELEBRATING,//
  MOOD_SAD, //
  MOOD_STUDY, //
};

MoodState mood_evaluate(uint8_t habits_done, uint16_t streak, uint8_t hour);