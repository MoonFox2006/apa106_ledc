#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "APA106.h"

#define LED_GPIO    GPIO_NUM_18

void app_main() {
    ESP_ERROR_CHECK(apa106_init(LED_GPIO));
    while (1) {
        uint16_t c;

        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(c, 0, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(0, c, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(0, 0, c);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(c, c, 0);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(c, 0, c);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(0, c, c);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        for (c = 1; c < 256; c = (c << 1) | 1) {
            apa106_rgb(c, c, c);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        apa106_rgb(0, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
