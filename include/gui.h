#ifndef GUI_H
#define GUI_H

#include <sstream>
#include <iomanip>
#include "edp.h"

#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/EBGaramond_VariableFont_wght14pt7b.h>
#include <Fonts/EBGaramond_VariableFont_wght10pt7b.h>
#include <Fonts/EBGaramond_VariableFont_wght8pt7b.h>
#include <Fonts/EBGaramond_VariableFont_wght24pt7b.h>
#include <Fonts/RobotoSlab_VariableFont_wght14pt7b.h>
#include <Fonts/RobotoSlab_VariableFont_wght24pt7b.h>

#include "moods/neutral.h"
#include "moods/sleeping.h"
#include "moods/happy.h"
#include "moods/celebrating.h"
#include "moods/sad.h"
#include "moods/satisfied.h"
#include "moods/worried.h"
#include "moods/thriving.h"
#include "moods/study.h"

// general functions
void init_gui();
void gui_main_menu();
void display_current_time();

// penguin param
const int penW = 150; 
const int penH = 150; 
const int initial_offset = 29; 

struct Penguin{
  int offset = initial_offset;
  int direction = 1;
  unsigned long lastTs = 0;
};

void update_penguin(int image_id);
void draw_penguin(uint16_t H_offset, int image_id);

// habit param
const int habit_fontsize = 10; //based on font
const int habit_spacing = 10; //based on font
const int habit_offset = initial_offset+penH+20;

void draw_habit_list();
void draw_habit(int cursor);

// pomo param
void gui_pomo_menu();
void draw_pomo(uint32_t seconds);
void draw_break(uint32_t seconds);

#endif