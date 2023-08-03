#include <Arduino.h>
#include <GyverHub.h>
#include <Stamp.h>

#include "data.h"
#include "GHub.h"

void setup() {
  Serial.begin(115200);

  hub_start(AP_SSID, AP_PASS, "CrazyDevices", "greenhouse", "");  
}

void loop() {
  hub_tick();
}
