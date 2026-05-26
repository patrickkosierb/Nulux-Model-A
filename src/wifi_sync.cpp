
#include "wifi_sync.h"
#include <WiFi.h>
#include "time.h"
#include "habit.h"
#include "gui.h"

static const char* NTP_SERVER   = "pool.ntp.org";
static const long  GMT_OFFSET_S = -14400;  // UTC-4 (Toronto, DST active)
static const int   DST_OFFSET_S = 0;

char ssid[] = "Linksys29188";
char pass[] = "isjbipzyfx";

bool syncTimeViaNTP() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 15000) {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      return false;
    }
    delay(500);
  }

  configTime(GMT_OFFSET_S, DST_OFFSET_S, NTP_SERVER);

  delay(5000);

  struct tm t;
  bool ok = getLocalTime(&t);
  save_timestamp();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  return ok;
}
