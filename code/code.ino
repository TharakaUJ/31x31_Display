#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include <Arduino.h>


void setup() {
    Serial.begin(115200);
    setupWeb();
    setupDisplay();
    Serial.println("Setup complete");
}


void loop() {
    // Call the web server loop function
    Serial.println("Looping...");
    delay(1000);
}