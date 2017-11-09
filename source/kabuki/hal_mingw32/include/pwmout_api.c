/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdafx.h>
#include "../../hal/include/mbed_assert.h"
#include "../../hal/include/pwmout_api.h"
#include "../../hal/include/pinmap.h"

#define TCR_CNT_EN       0x00000001
#define TCR_RESET        0x00000002

//  PORT ID, PWM ID, Pin function
static const PinMap PinMap_PWM[] = {
    {P1_18, PWM_1, 2},
    {P1_20, PWM_2, 2},
    {P1_21, PWM_3, 2},
    {P1_23, PWM_4, 2},
    {P1_24, PWM_5, 2},
    {P1_26, PWM_6, 2},
    {P2_0 , PWM_1, 1},
    {P2_1 , PWM_2, 1},
    {P2_2 , PWM_3, 1},
    {P2_3 , PWM_4, 1},
    {P2_4 , PWM_5, 1},
    {P2_5 , PWM_6, 1},
    {P3_24, PWM_1, 3},
    {P3_25, PWM_2, 3},
    {P3_26, PWM_3, 3},
    {P3_27, PWM_4, 3},
    {P3_28, PWM_5, 3},
    {P3_29, PWM_6, 3},
    {NC, NC, 0}
};

uint32_t *PWM_MATCH[] = {
    0,
    0
};

#define TCR_PWM_EN       0x00000008

static unsigned int pwm_clock_mhz;

void pwmout_init(pwmout_t* obj, PinName pin) {
}

void pwmout_free(pwmout_t* obj) {
    // [TODO]
}

void pwmout_write(pwmout_t* obj, float value) {
}

float pwmout_read(pwmout_t* obj) {
    return 0.0f;
}

void pwmout_period(pwmout_t* obj, float seconds) {
    pwmout_period_us(obj, seconds * 1000000.0f);
}

void pwmout_period_ms(pwmout_t* obj, int ms) {
    pwmout_period_us(obj, ms * 1000);
}

// Set the PWM period, keeping the duty cycle the same.
void pwmout_period_us(pwmout_t* obj, int us) {
}

void pwmout_pulsewidth(pwmout_t* obj, float seconds) {
    pwmout_pulsewidth_us(obj, seconds * 1000000.0f);
}

void pwmout_pulsewidth_ms(pwmout_t* obj, int ms) {
    pwmout_pulsewidth_us(obj, ms * 1000);
}

void pwmout_pulsewidth_us(pwmout_t* obj, int us) {
}
