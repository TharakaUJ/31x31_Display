#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include <Arduino.h>
#include "menu.h"

void setup()
{
    Serial.begin(115200);
    setupWeb();
    setupDisplay();
    Serial.println("Setup complete");

    delay(2000);
    Serial.println("Starting game...");
    delay(1000);
    menu_init();

    framebuffer[0][0] = CRGB::Red;
    framebuffer[1][1] = CRGB::Green;
    framebuffer[2][2] = CRGB::Blue;
}

void loop()
{
    // Serial.println("Looping...");
    // delay(5000);
    // server.handleClient();
}