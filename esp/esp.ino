#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include "menu.h"


void setup() {
    Serial.begin(115200);
    setupWeb();
    setupDisplay();
    Serial.println("Setup complete");

    delay(2000);
    Serial.println("Starting game...");
    delay(1000);
    setupSnakeGame();
}


void loop() {
    // Serial.println("Looping...");
    // delay(5000);
    // server.handleClient();
}