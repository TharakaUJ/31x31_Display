#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include "game.h"
#include <Arduino.h>


void setup() {
    Serial.begin(115200);
    setupWeb();
    setupDisplay();
    Serial.println("Setup complete");

    setupSnakeGame();
}


void loop() {
    Serial.println("Looping...");
    delay(1000);
}