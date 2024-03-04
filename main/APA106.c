#include <esp_attr.h>
#include <soc/ledc_reg.h>
#include <driver/ledc.h>
#include "APA106.h"

#define APA_COUNT   1

static struct __attribute__((__packed__)) {
    uint8_t rgb[APA_COUNT * 3];
    volatile uint8_t bitpos;
} _apa_data;

static IRAM_ATTR void ledc_isr(void *arg) {
    uint32_t st = READ_PERI_REG(LEDC_INT_ST_REG);

    if (st & LEDC_OVF_CNT_CH0_INT_ST) {
        WRITE_PERI_REG(LEDC_CH0_CONF0_REG, (READ_PERI_REG(LEDC_CH0_CONF0_REG) & ~(LEDC_OVF_NUM_CH0_M)) | LEDC_OVF_CNT_RESET_CH0);
        if (_apa_data.bitpos < APA_COUNT * 24) {
            if (_apa_data.rgb[_apa_data.bitpos >> 3] & (0x80 >> (_apa_data.bitpos & 0x07)))
                WRITE_PERI_REG(LEDC_CH0_DUTY_REG, 15 << 4);
            else
                WRITE_PERI_REG(LEDC_CH0_DUTY_REG, 4 << 4);
            WRITE_PERI_REG(LEDC_CH0_CONF0_REG, READ_PERI_REG(LEDC_CH0_CONF0_REG) | LEDC_PARA_UP_CH0);
            ++_apa_data.bitpos;
        } else {
            WRITE_PERI_REG(LEDC_CH0_DUTY_REG, 0);
            WRITE_PERI_REG(LEDC_CH0_CONF0_REG, (READ_PERI_REG(LEDC_CH0_CONF0_REG) & ~(LEDC_OVF_CNT_EN_CH0)) | LEDC_PARA_UP_CH0);
        }
    }
    WRITE_PERI_REG(LEDC_INT_CLR_REG, st);
}

esp_err_t IRAM_ATTR apa106_init(gpio_num_t pin) {
    esp_err_t result;

    result = ledc_isr_register(&ledc_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    if (result == ESP_OK) {
        const ledc_timer_config_t timer_cfg = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_4_BIT,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 714286,
            .clk_cfg = LEDC_AUTO_CLK,
            .deconfigure = false
        };

        result = ledc_timer_config(&timer_cfg);
        if (result == ESP_OK) {
            const ledc_channel_config_t channel_cfg = {
                .gpio_num = pin,
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = LEDC_CHANNEL_0,
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = LEDC_TIMER_0,
                .duty = 0,
                .hpoint = 0,
                .flags.output_invert = false
            };

            result = ledc_channel_config(&channel_cfg);
            if (result == ESP_OK) {
                SET_PERI_REG_MASK(LEDC_INT_ENA_REG, LEDC_OVF_CNT_CH0_INT_ENA);
            }
        }
    }
    return result;
}

void IRAM_ATTR apa106_rgb(uint8_t r, uint8_t g, uint8_t b) {
    _apa_data.rgb[0] = r;
    _apa_data.rgb[1] = g;
    _apa_data.rgb[2] = b;
    _apa_data.bitpos = 0;
    WRITE_PERI_REG(LEDC_CH0_DUTY_REG, 0);
    WRITE_PERI_REG(LEDC_CH0_CONF1_REG, 0);
    SET_PERI_REG_MASK(LEDC_CH0_CONF0_REG, LEDC_OVF_CNT_RESET_CH0 | LEDC_OVF_CNT_EN_CH0 | (35 << LEDC_OVF_NUM_CH0_S) | LEDC_PARA_UP_CH0);
}
