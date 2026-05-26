#include "hw.h"

#define BUZZ_CHANNEL 4
#define BUZZ_FREQ    2000   // Hz - tune for best tone
#define BUZZ_RES     8      // bits

volatile int button_state = BUTTON_DEFUALT;
unsigned long bot_press_start = 0;
unsigned long lastPressTime = 0;
const int debounceDelay = 250;
const int button_held = 2000;

void init_hw(void){
    Serial.begin(115200);
    // gpio
    pinMode(ONBOARD_LED,OUTPUT);
    pinMode(LIFE_LED,OUTPUT);
    //buzzer 
    ledcSetup(BUZZ_CHANNEL, BUZZ_FREQ, BUZZ_RES);
    ledcAttachPin(BUZZER, BUZZ_CHANNEL);
    ledcWrite(BUZZ_CHANNEL, 0);
    // buttons
    pinMode(RIGHT_BUTTON, INPUT_PULLUP);  
    attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON), rHandleISR, FALLING);
    pinMode(MID_BUTTON, INPUT_PULLUP);  
    attachInterrupt(digitalPinToInterrupt(MID_BUTTON), midHandleISR, CHANGE);
    pinMode(LEFT_BUTTON, INPUT_PULLUP);  
    attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON),  lHandleISR, FALLING);
}

void IRAM_ATTR rHandleISR() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > debounceDelay) {
        button_state = RIGHT_PRESSED;
        lastPressTime = currentTime;
    }
}
void IRAM_ATTR lHandleISR() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > debounceDelay) {
        button_state = LEFT_PRESSED;
        lastPressTime = currentTime;
    }
}

void IRAM_ATTR midHandleISR() {
    unsigned long currentTime = millis();
    if (digitalRead(MID_BUTTON) == LOW) {
        bot_press_start = currentTime;
        Serial.println("pressedd");
    } else {
        // rising edge - debounce the release
        if (currentTime - lastPressTime > debounceDelay) {
            lastPressTime = currentTime;
            unsigned long held = currentTime - bot_press_start;
            button_state = (held >= button_held) ? MID_HELD : MID_PRESSED;
        }
    }
}

void buzz_bling() {
    int notes[][2] = {
        {1047, 60},   // C6
        {1319, 60},   // E6
        {1568, 80},   // G6
        {2093, 150},  // C7 - held
    };

    int numNotes = sizeof(notes) / sizeof(notes[0]);

    for (int i = 0; i < numNotes; i++) {
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(15);
    }
}
void buzz_victory() {
    int notes[][2] = {
        // zelda secret/puzzle solved jingle
        {1319, 80},   // E6
        {1319, 80},   // E6
        {1319, 80},   // E6
        {1319, 300},  // E6 - held
        {0,    60},   // rest
        {1047, 80},   // C6
        {1175, 80},   // D6
        {1319, 80},   // E6
        {1568, 400},  // G6 - held resolution
        {0,    80},   // rest
        // chest open rising
        {784,  60},   // G5
        {1047, 60},   // C6
        {1319, 60},   // E6
        {1568, 60},   // G6
        {1760, 60},   // A6
        {1976, 60},   // B6
        {2093, 500},  // C7 - long triumphant hold
    };

    int numNotes = sizeof(notes) / sizeof(notes[0]);

    for (int i = 0; i < numNotes; i++) {
        if (notes[i][0] == 0) {
            delay(notes[i][1]);
            continue;
        }
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(15);
    }
}

void buzz_startup() {
    // {frequency Hz, duration ms}
    int notes[][2] = {
        {523,  80},   // C5
        {659,  80},   // E5
        {784,  80},   // G5
        {1047, 120},  // C6
        {784,  60},   // G5
        {1047, 80},   // C6
        {1319, 80},   // E6
        {1568, 80},   // G6
        {2093, 150},  // C7
        {2637, 200},  // E7  - held ending note
    };

    int numNotes = sizeof(notes) / sizeof(notes[0]);

    for (int i = 0; i < numNotes; i++) {
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(20);
    }
}

void buzz_pomo_start() {
    int notes[][2] = {
        {784,  60},   // G5  — first strike
        {0,    40},   // rest
        {784,  60},   // G5  — repeat
        {0,    40},   // rest
        {1047, 80},   // C6  — step up
        {1319, 80},   // E6  — step up
        {1568, 180},  // G6  — held resolve: "you're locked in"
    };
    int n = sizeof(notes) / sizeof(notes[0]);
    for (int i = 0; i < n; i++) {
        if (notes[i][0] == 0) {
            delay(notes[i][1]);
            continue;
        }
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(15);
    }
}

void buzz_pomo_break() {
    int notes[][2] = {
        {1568, 100},  // G6  — high start
        {1319, 80},   // E6  — step down
        {1047, 80},   // C6  — step down
        {0,    50},   // rest — breath
        {784,  60},   // G5  — soft land
        {659,  200},  // E5  — long settle: "relax now"
    };
    int n = sizeof(notes) / sizeof(notes[0]);
    for (int i = 0; i < n; i++) {
        if (notes[i][0] == 0) {
            delay(notes[i][1]);
            continue;
        }
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(15);
    }
}

void buzz_pomo_resume() {
    int notes[][2] = {
        {659,  50},   // E5  — low pickup
        {784,  50},   // G5  — climb
        {1047, 70},   // C6  — mid land
        {0,    30},   // rest — tiny breath
        {1319, 160},  // E6  — held: "back at it"
    };
    int n = sizeof(notes) / sizeof(notes[0]);
    for (int i = 0; i < n; i++) {
        if (notes[i][0] == 0) {
            delay(notes[i][1]);
            continue;
        }
        ledcSetup(BUZZ_CHANNEL, notes[i][0], BUZZ_RES);
        ledcAttachPin(BUZZER, BUZZ_CHANNEL);
        ledcWrite(BUZZ_CHANNEL, 128);
        delay(notes[i][1]);
        ledcWrite(BUZZ_CHANNEL, 0);
        delay(15);
    }
}

