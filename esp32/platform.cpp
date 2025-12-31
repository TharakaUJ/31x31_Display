#include "../common/platform.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <FastLED.h>

extern CRGB leds[];

void createThread(const char* name, ThreadFunction function, void* params, 
                  int stackSize, int priority)
{
    // Wrap the function pointer to match FreeRTOS task signature
    xTaskCreatePinnedToCore(
        function,
        name,
        stackSize,
        params,
        priority,
        NULL,
        1  // Pin to core 1
    );
}

void platformDelay(int milliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void platformDeleteCurrentThread()
{
    vTaskDelete(NULL);
}

void platformInitDisplay()
{
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
}

void platformShowDisplay()
{
    FastLED.show();
}
