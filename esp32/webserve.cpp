#include <WebServer.h>
#include <WiFi.h>
#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include "htmlFiles.h"
#include <sstream>

WebServer server(80);

void handleRoot()
{
    server.send(200, "text/html", root);
}

void handleUp()
{
    server.send(200, "text/plain", "Up");
    up();
}

void handleDown()
{
    server.send(200, "text/plain", "Down");
    down();
}

void handleLeft()
{
    server.send(200, "text/plain", "Left");
    left();
}

void handleRight()
{
    server.send(200, "text/plain", "Right");
    right();
}

void handleSelect()
{
    server.send(200, "text/plain", "Select");
    select();
}

void handleBack()
{
    server.send(200, "text/plain", "Back");
    back();
}

void handleSimulate()
{
    Serial.print("simulate");

    std::string response = simulate;

    server.send(200, "text/html", response.c_str());
}

void handleFramebuffer()
{
    String json = "[";
    for (int y = 0; y < 31; y++)
    {
        for (int x = 0; x < 31; x++)
        {
            CRGB color = framebuffer[x][y];
            json += String("{\"x\":") + x + ",\"y\":" + y +
                    ",\"r\":" + color.r + ",\"g\":" + color.g +
                    ",\"b\":" + color.b + "},";
        }
    }
    if (json.endsWith(","))
        json.remove(json.length() - 1); // remove trailing comma
    json += "]";
    server.send(200, "application/json", json);
}

void setupWeb()
{

    // // Connect to external Wi-Fi network
    // const char *ssid = "Disconnected";
    // const char *password = "whybother";

    // WiFi.begin(ssid, password);

    // // Wait for connection
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }

    // Serial.println("Connected to WiFi");

    // Create an access point
    const char *ssid = "ESP32_AP";
    const char *password = "12345678";

    WiFi.softAP(ssid, password);

    // Enable low power mode
    WiFi.setSleep(true);
    WiFi.setTxPower(WIFI_POWER_MINUS_1dBm); // Options: 19.5, 15, 11, 8.5, 7, 5, 2, -1 (WIFI_POWER_MINUS_1dBm) dBm

    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    // Web server setup
    server.on("/", handleRoot);
    server.on("/simulate", handleSimulate);
    server.on("/framebuffer", handleFramebuffer);
    server.on("/up", handleUp);
    server.on("/down", handleDown);
    server.on("/left", handleLeft);
    server.on("/right", handleRight);
    server.on("/select", handleSelect);
    server.on("/back", handleBack);
    server.on("/center", handleSelect);
    server.onNotFound([]()
                      { server.send(404, "text/plain", "404: Not Found-This is the esp32 web server"); });
    server.begin();

    Serial.println("HTTP server started");
    Serial.println("Server IP: " + WiFi.localIP().toString());
    loopWeb();
    Serial.println("Web server loop started");
}

void loopWeb()
{
    static TaskHandle_t serverTaskHandle = NULL;

    if (serverTaskHandle == NULL)
    {
        xTaskCreate(
            [](void *param)
            {
                for (;;)
                {
                    server.handleClient();
                    vTaskDelay(1); // Yield to other tasks
                }
            },
            "WebServerTask",
            4096, // Stack size
            NULL,
            1, // Priority
            &serverTaskHandle);
    }
}
