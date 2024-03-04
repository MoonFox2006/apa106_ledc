#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/gpio.h>

esp_err_t apa106_init(gpio_num_t pin);
void apa106_rgb(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif
