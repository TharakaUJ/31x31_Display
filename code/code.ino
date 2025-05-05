#include "webserve.h"
#include "controllerEndpoints.h"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    setupWeb();
}


void loop() {
    // Call the web server loop function
    Serial.println("Looping...");
    delay(1000);
}