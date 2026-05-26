#include "gui.h"
#include "habit.h"
#include "state.h"

GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // my suggested wiring and proto board

static Penguin penguin;

// USING MOOD ENGINE
const uint8_t* penguin_animations[] = {
    epd_bitmap_sleep,
    epd_bitmap_neutral,
    epd_bitmap_worried,
    epd_bitmap_satisfied,
    epd_bitmap_happy,
    epd_bitmap_thriving,
    epd_bitmap_celebrating,
    epd_bitmap_sad,
    epd_bitmap_study,
};

// USING STATE FUNCTION
// const uint8_t* penguin_animations[] = {
//     epd_bitmap_sleep,
//     epd_bitmap_neutral,
//     epd_bitmap_happy,
//     epd_bitmap_sad,
//     epd_bitmap_celebrating,
// };

// initialize display
void init_gui(){
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse
  display.setRotation(1);
  // display.setFont(&EBGaramond_VariableFont_wght14pt7b);
  display.setFont(&RobotoSlab_VariableFont_wght14pt7b);

  display.setTextColor(GxEPD_BLACK);
  gui_main_menu();
  display.hibernate();
}

// main menu
void gui_main_menu()
{
  struct tm t;
  // habit list start screen
  const char main_menu_txt[] = "habits";
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(main_menu_txt, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t tx = ((display.width() - tbw) / 2) - tbx;
  uint16_t ty = tbh; 
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(tx, ty);
    display.print(main_menu_txt);
    draw_habit_list();
  }
  while (display.nextPage());

  sleep(1); // time for start screen active 
  
  // main habits screen
  display_current_time();
  do
  {
    // update_penguin(check_state(habits));
    getLocalTime(&t);
    update_penguin(mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour));
    update_penguin(mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour)); //test
    draw_habit(0);

  }
  while (display.nextPage());
}


// displays time ontop of penguin
void display_current_time()
{
  struct tm timeinfo;
  char time_str[16];
  if (getLocalTime(&timeinfo)) {
    strftime(time_str, sizeof(time_str), "%H:%M", &timeinfo);
  } else {
    strcpy(time_str, "--:--");
  }

  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(time_str, 0, 0, &tbx, &tby, &tbw, &tbh);
  // uint16_t tx = ((display.width() - tbw) / 2) - tbx;
  uint16_t tx = 0;

  uint16_t ty = tbh;
  uint16_t row_h = tbh + 4;

  display.setPartialWindow(0, 0, display.width(), row_h);
  display.firstPage();
  do
  {
    display.fillRect(0, 0, display.width(), row_h, GxEPD_WHITE); // explicit clear
    display.setCursor(tx, ty);
    display.print(time_str);
  }
  while (display.nextPage());
}

// moves penguin up and down 
void update_penguin(int image_id){  

  // if (millis() - penguin.lastTs > 800) {
  //     penguin.lastTs = millis();
  //     // move up and down by 2px
  //     penguin.offset += penguin.direction;
  //     if (penguin.offset >= initial_offset+1 ||  penguin.offset <=  initial_offset) {penguin.direction = -penguin.direction;}
  draw_penguin(penguin.offset, image_id);
  // }
}

// draws penguin depending on state
void draw_penguin(uint16_t H_offset,  int image_id){

  uint16_t px = (display.width()  - penW)/2;  // centered horizontally

  display.setPartialWindow(px, H_offset, display.width(), penH); //penW
  display.firstPage();
  do
  {
    display.fillRect(px, H_offset, display.width(), penH, GxEPD_WHITE); // clear just this area
    display.drawBitmap(px, H_offset, penguin_animations[image_id], penW, penH, GxEPD_BLACK);
  }
  while (display.nextPage());

}


void draw_habit_list() {
    uint16_t font = 14; //based on font
    int16_t hbx,hby; uint16_t hbw,hbh;
  
    for (int i = 0; i < NUM_HABITS; i++) {
        
        display.getTextBounds(habits[i].name, 0, 0, &hbx, &hby, &hbw, &hbh);
        uint16_t hx = ((display.width() - hbw) / 2) - hbx;

        display.setCursor(hx, 20+initial_offset+(habit_spacing+habit_fontsize)*i);
        display.print(habits[i].name);

        display.setCursor(hx+hbw+10, 20+initial_offset+(habit_spacing+habit_fontsize)*i);
        display.print(habits[i].is_done ? "x" : "-");

    }
}

void draw_habit(int cursor) {

  int16_t hbx,hby; uint16_t hbw,hbh;
  display.setPartialWindow(0, habit_offset-20, display.width(), NUM_HABITS*(habit_fontsize+habit_spacing));
  display.firstPage();
  do
  {
    display.fillRect(0, habit_offset-20, display.width(), NUM_HABITS*(habit_fontsize+habit_spacing), GxEPD_WHITE);
    display.getTextBounds(habits[cursor].name, 0, 0, &hbx, &hby, &hbw, &hbh);
    uint16_t hx = ((display.width() - hbw) / 2) - hbx;

    display.setCursor(hx, habit_offset);
    display.print(habits[cursor].name);

    display.setCursor(hx + hbw + 10, habit_offset);
    display.print(habits[cursor].is_done ? "x" : "-");
    display.setCursor(0, habit_offset);
    display.print(habits[cursor].streak);
  }
  while (display.nextPage());

}


// POMODORO RELATED

void gui_pomo_menu(){
  draw_pomo(pomo_time);
  display_current_time();
}

void draw_pomo(uint32_t seconds){
  String desited_pomo = pomo_get_time(seconds);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.setFont(&RobotoSlab_VariableFont_wght24pt7b);
  display.getTextBounds(String(desited_pomo), 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  // display.setPartialWindow(0, 0, display.width(), display.height());
  // display.setPartialWindow(x,y-tbh/2,tbw+3,tbh);
  display.setPartialWindow(0, 30, display.width(), display.height());
  do
  { 
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(String(desited_pomo));
  }
  while (display.nextPage());
  display.setFont(&RobotoSlab_VariableFont_wght14pt7b);
}

void draw_break(uint32_t seconds){
  display.setFont(&RobotoSlab_VariableFont_wght14pt7b);
  String desited_pomo = pomo_get_time(seconds);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(String(desited_pomo), 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  display.setPartialWindow(0, habit_offset-20, display.width(), display.height());
  display.firstPage();
  do
  {
    display.fillRect(0, habit_offset-20, display.width(), display.height(), GxEPD_WHITE);
    display.setCursor(x, habit_offset);
    display.print(String(desited_pomo));

  }
  while (display.nextPage());
  display.setFont(&RobotoSlab_VariableFont_wght24pt7b);

}



