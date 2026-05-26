#include "pomo.h"

bool pomo_running = false;
uint32_t pomo_time = 0;
TaskHandle_t      pomo_task_handle = NULL;


void pomo_task(void* param){  

  buzz_pomo_start();
  int epoch = pomo_time/300;
  int break_count;
  epoch--;
  // run timer
  while(pomo_running && pomo_time>0){ 
    
    // update screen every min
    if(pomo_time%60==0){
      draw_pomo(pomo_time);
    }

    delay(1000);
    pomo_time--;
    
    // break block 
    if(pomo_time%(30*60)==0 && pomo_time!=0){

      epoch--;
      Serial.print("Break time! Epoch remaining: ");
      Serial.println(epoch);
      buzz_pomo_break();
      break_count = 5*60;

      // start break
      while(pomo_running && break_count>0){
        if(break_count%60==0){
          update_penguin(MOOD_STUDY);
          draw_break(break_count);
        }
        delay(1000);
        break_count--;
      }
      Serial.println("Break end!");
      buzz_pomo_resume();
    }

  }
  // reset parameters
  Serial.println("Stopping Pomodoro timer");
  pomo_running = false;
  pomo_task_handle  = NULL;
  epoch = 0;
  draw_pomo(pomo_time);

  vTaskDelete(NULL);  // task cleans itself up

}


void pomo_start() {

    if (pomo_time == 0) {
        ESP_LOGW(TAG, "pomo_start called with pomo_time == 0, ignoring");
        return;
    }

    pomo_running = true;

    xTaskCreatePinnedToCore(
        pomo_task,          // task function
        "pomo_task",        // name (shows in task list)
        4096,               // stack in bytes — increase if draw_* allocates heavily
        NULL,               // param
        2,                  // priority (above idle, below display/input if those are 3+)
        &pomo_task_handle,  // handle for later inspection
        1                   // core 1 — leaves core 0 free for WiFi/BLE if needed
    );
}

void pomo_stop() {
    pomo_running = false;
    buzz_pomo_break();
}

void pomo_inc(){
  if(pomo_time>=43200)
    pomo_time=0;
  else
    pomo_time = pomo_time+1800;
}

void pomo_dec(){
  if(pomo_time<=0)
    pomo_time=0;
  else
    pomo_time = pomo_time-1800;
}

String pomo_get_time(uint32_t seconds){
  int minutes = (seconds/60) % 60;
  int hours = seconds/3600;
  //int remaining = seconds % 60; //for seconds display
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << minutes; //  << ":"  << std::setfill('0') << remaining; 
  std::string formattedTime = oss.str();
  Serial.println(formattedTime.c_str());
  return formattedTime.c_str();
}

