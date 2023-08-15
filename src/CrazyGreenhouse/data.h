#include <EEPROM.h>
#include <EEManager.h>

//#define LOAD_DATA EEPROM.get(0, db)
//#define SAVE_DATA EEPROM.put(0, db);EEPROM.commit()

#define RESET ESP.reset()

/*void LOG(String text) {
  Serial.println(text);
  hub.print(text, GH_RED);
}*/

// WiFi
#define AP_SSID "MiHome"
#define AP_PASS "!vlad-02072015!"

GHbutton b1, b2, reset;

bool w1, w2;

float porpol;
GHbutton p;

float t = 28.52;
float h = 42.49;
float u = 70;

struct DB {
  const char* wssid;
  const char* wpass;
};

DB db;
EEManager memory(db);