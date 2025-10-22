
#include <Arduino.h>
#include "driver/ledc.h"
#include "driver/timer.h"

#include "pitch2frq.h"

#define SAMPLE_RATE 40000 // Weird! I know... but it makes sense pitch-wise
#define NUM_CHANNELS 6
#define PWM_RESOLUTION 10
#define PWM_FREQUENCY 78125

const int pwm_pins[NUM_CHANNELS] = { 2, 4, 16, 17, 5, 18 }; // Example pins, change as needed

#define PWM_TIMER LEDC_TIMER_0
#define TIMER_GROUP TIMER_GROUP_0
#define TIMER_IDX TIMER_0
#define TIMER_DIVIDER 80 // 80MHz / 80 = 1MHz

static intr_handle_t s_timer_handle;

uint32_t phase = 0;
uint32_t frq = pitchToFrequency((9 + 12*3) << 16); // A4 ?
uint16_t out = 0;

void IRAM_ATTR timer_isr(void *para) {


    phase += frq;
    out = (phase >> 10) & 0x03FF;

    if (out > 511) {
        out = 1023 - out; // Reflect over the midpoint for falling edge
    }

    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, out);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);


    timer_group_clr_intr_status_in_isr(TIMER_GROUP, TIMER_IDX);
    timer_group_enable_alarm_in_isr(TIMER_GROUP, TIMER_IDX);
}

void setup() {
    // LEDC timer config
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = (ledc_timer_bit_t)PWM_RESOLUTION,
        .timer_num = PWM_TIMER,
        .freq_hz = PWM_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // LEDC channel config
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        ledc_channel_config_t ledc_channel = {
            .gpio_num = pwm_pins[ch],
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = (ledc_channel_t)ch,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = PWM_TIMER,
            .duty = 0,
            .hpoint = 0
        };
        ledc_channel_config(&ledc_channel);
    }

    // Timer config
    timer_config_t timer_config = {
        .alarm_en = TIMER_ALARM_EN,
        .counter_en = TIMER_PAUSE,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = TIMER_DIVIDER
    };
    timer_init(TIMER_GROUP, TIMER_IDX, &timer_config);
    timer_set_alarm_value(TIMER_GROUP, TIMER_IDX, 1000000 / SAMPLE_RATE);
    timer_enable_intr(TIMER_GROUP, TIMER_IDX);
    timer_isr_register(TIMER_GROUP, TIMER_IDX, timer_isr, NULL, ESP_INTR_FLAG_IRAM, &s_timer_handle);
    timer_start(TIMER_GROUP, TIMER_IDX);
}


void loop() {
}