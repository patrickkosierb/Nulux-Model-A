#include "state.h"

extern volatile int button_state;   
bool screen_state = STATE_MENU;
int cursor_pos = 0;
int pen_state = 0;
struct tm t;

void state_machine(){

    switch(button_state){

        case RIGHT_PRESSED:
        button_state=BUTTON_DEFUALT;
        getLocalTime(&t);

        if(screen_state==STATE_MENU){
            Serial.println("RIGHT BUTTON");  
            cursor_pos++;
            // pen_state = check_state(habits);
            // pen_state = mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour);
            // update_penguin(pen_state);
            
            if(cursor_pos>NUM_HABITS-1){
                cursor_pos=0;
            }
            draw_habit(cursor_pos);
        }else{
            pomo_inc();
            draw_pomo(pomo_time);
        }
        break;

        case LEFT_PRESSED:
        button_state=BUTTON_DEFUALT;
        getLocalTime(&t);

        if(screen_state==STATE_MENU){
            Serial.println("LEFT BUTTON");
            cursor_pos--;
            // pen_state = check_state(habits);
            // pen_state = mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour);        
            // update_penguin(pen_state);
            if(cursor_pos<0){
                cursor_pos=NUM_HABITS-1;
            }
            draw_habit(cursor_pos);
        }else{
            pomo_dec();
            draw_pomo(pomo_time);
        }
        break;

        case MID_PRESSED:
        button_state=BUTTON_DEFUALT;
        bool was_done;
        if(screen_state==STATE_MENU){
            Serial.println("MID BUTTON");
            was_done = habits[cursor_pos].is_done;
            habits[cursor_pos].is_done = !habits[cursor_pos].is_done;
            if(!was_done){
                habits[cursor_pos].streak++;
            }else{
                habits[cursor_pos].streak--;
            }
            save_state();
            draw_habit(cursor_pos);

            getLocalTime(&t);
            // pen_state = check_state(habits);
            pen_state = mood_evaluate(habits_completed(habits), habits_avg_streak(), t.tm_hour);
            update_penguin(pen_state);

            if (!was_done && habits[cursor_pos].is_done && habits_completed(habits)==NUM_HABITS) {
                buzz_victory();
                update_penguin(MOOD_CELEBRATING);
                sleep(5);
                update_penguin(MOOD_SATISFIED);
            }else if(!was_done && habits[cursor_pos].is_done){
                buzz_bling();
            }


        }else{ //start pomo
            if(pomo_running){
                pomo_stop();
                delay(500);
                pomo_time=0;
                draw_pomo(pomo_time);
                //  INDICATE SOME PAUSED TEXT
            }else{
                draw_pomo(pomo_time);
                pomo_start(); //TODO: Make this a thread so its not blocking pauses
            }


        }

        break;

        case MID_HELD:
        button_state=BUTTON_DEFUALT;
        Serial.println("HELD");

        if(screen_state==STATE_MENU){
            gui_pomo_menu();
            screen_state = STATE_POMO;
        }else{
            gui_main_menu();
            screen_state=STATE_MENU;
        }

        break;

        default:
        button_state=BUTTON_DEFUALT;
        break;
    }

}   

