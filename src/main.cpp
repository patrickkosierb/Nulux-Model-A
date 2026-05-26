#include "state.h"
#include "wifi_sync.h"

static bool rolled_over = false;

void time_check_task(void* param) {
    while (true) {
        struct tm t;
        getLocalTime(&t);

        if (t.tm_hour == 0 && !rolled_over) {
            check_day_rollover();
            rolled_over = true;
        }
        if (t.tm_hour != 0) {
            rolled_over = false;
        }
        save_timestamp();
        display_current_time();
        if(screen_state==STATE_MENU)
            update_penguin(mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour));
        vTaskDelay(pdMS_TO_TICKS(60000));  // check once per minute
    }
}

void setup() {
  init_hw();
  
  load_timestamp(); 
  if (!syncTimeViaNTP()) {
    Serial.println("[WiFi] Time sync failed, using stored time");
  }

  struct tm t;
  getLocalTime(&t);
  Serial.printf("Time: %02d:%02d:%02d  %02d/%02d/%04d\n",
  t.tm_hour, t.tm_min, t.tm_sec,
  t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);

  load_state();   // restore habits 
  init_gui(); 
//   buzz_startup();
  
  xTaskCreate(time_check_task, "time_check", 2048, nullptr, 1, nullptr);  // ← here
}

void loop(){
  state_machine();
};
