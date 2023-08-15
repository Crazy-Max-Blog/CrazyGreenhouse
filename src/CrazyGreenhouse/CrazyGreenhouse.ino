#include <Arduino.h>
#include <GyverHub.h>
#include <Stamp.h>

#include "data.h"
#include "GHub.h"

void setup() {
    EEPROM.begin(memory.blockSize());
    memory.begin(0, 'b');
  
  //LOAD_DATA;

  Serial.begin(115200);

  hub_start(db.wssid, db.wpass, "CrazyDevices", "greenhouse", "");  
}

void loop() {
  hub_tick();
}
