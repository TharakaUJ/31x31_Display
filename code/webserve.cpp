#include <WebServer.h>
#include <WiFi.h>
#include "webserve.h"
#include "controllerEndpoints.h"
#include "display.h"
#include <string>


WebServer server(80);

void handleRoot()
{
    server.send(200, "text/html", R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 Control</title>
            <style>
                body {
                    display: flex;
                    flex-direction: column;
                    align-items: center;
                    justify-content: center;
                    height: 100vh;
                    background-color: lightblue;
                    margin: 0;
                }
                .button-container {
                    display: grid;
                    grid-template-columns: 100px;
                    grid-template-rows: 100px 100px 100px;
                    gap: 10px;
                }
                button {
                    width: 100px;
                    height: 100px;
                    font-size: 20px;
                    cursor: pointer;
                }
            </style>
        </head>
        <body>
            <h1>ESP32 Control Panel</h1>
            <div class="button-container">
                <button onclick="sendRequest('/up')">Up</button>
                <button onclick="sendRequest('/left')">Left</button>
                <button onclick="sendRequest('/right')">Right</button>
                <button onclick="sendRequest('/down')">Down</button>
            </div>
            <script>
                function sendRequest(endpoint) {
                    fetch(endpoint).then(response => {
                        if (response.ok) {
                            console.log(endpoint + " request successful");
                        } else {
                            console.error(endpoint + " request failed");
                        }
                    }).catch(error => console.error("Error:", error));
                }
            </script>
        </body>
        </html>
    )rawliteral");
}

void handleUp() {
    server.send(200, "text/plain", "Up");
    up();
}

void handleDown() {
    server.send(200, "text/plain", "Down");
    down();
}

void handleLeft() {
    server.send(200, "text/plain", "Left");
    left();
}

void handleRight() {
    server.send(200, "text/plain", "Right");
    right();
}

void handleSimulate() {
    std::string response = R"rawliteral(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Frame Buffer</title>
            <style>
                .grid {
                    display: grid;
                    grid-template-columns: repeat(31, 20px);
                    grid-gap: 1px;
                }
                .cell {
                    width: 20px;
                    height: 20px;
                }
            </style>
        </head>
        <body>
            <h1>Frame Buffer Display</h1>
            <div class="grid">
    )rawliteral";

    for (int y = 0; y < 31; ++y) {
        for (int x = 0; x < 31; ++x) {
            char cell[128];
            CRGB color = framebuffer[y][x];
            snprintf(cell, sizeof(cell), 
                     R"rawliteral(<div class="cell" style="background-color: #%06X;"></div>)rawliteral", 
                     color & 0xFFFFFF); // Assuming color is in 0xRRGGBB format
            response += cell;
        }
    }

    response += R"rawliteral(
            </div>
        </body>
        </html>
    )rawliteral";

    server.send(200, "text/html", response.c_str());
}

void setupWeb()
{
    Serial.begin(115200);

    // Connect to external Wi-Fi network
    const char *ssid = "Disconnected";
    const char *password = "whybother";

    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    // Web server setup
    server.on("/", handleRoot);
    server.on("/simulate", handleSimulate);
    server.on("/up", handleUp);
    server.on("/down", handleDown);
    server.on("/left", handleLeft);
    server.on("/right", handleRight);
    server.onNotFound([]() {
        server.send(404, "text/plain", "404: Not Found-This is the esp32 web server");
    });
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
            [](void *param) {
                for (;;)
                {
                    server.handleClient();
                    vTaskDelay(1); // Yield to other tasks
                }
            },
            "WebServerTask",
            4096, // Stack size
            NULL,
            1,    // Priority
            &serverTaskHandle);
    }
}
