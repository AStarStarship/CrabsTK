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
#include "../../hal/include/analogin_api.h"
#include "../../hal/include/pinmap.h"

#define ANALOGIN_MEDIAN_FILTER      1

#define ADC_10BIT_RANGE             0x3FF
#define ADC_12BIT_RANGE             0xFFF

static inline int div_round_up(int x, int y) {
  return (x + (y - 1)) / y;
}

static const PinMap PinMap_ADC[] = {
    {P0_23, ADC0_0, 1},
    {P0_24, ADC0_1, 1},
    {P0_25, ADC0_2, 1},
    {P0_26, ADC0_3, 1},
    {P1_30, ADC0_4, 3},
    {P1_31, ADC0_5, 3},
    {P0_12, ADC0_6, 3},
    {P0_13, ADC0_7, 3},
    {NC,    NC,     0}
};

#define ADC_RANGE    ADC_10BIT_RANGE


void analogin_init(analogin_t *obj, PinName pin) {
}

static inline uint32_t adc_read(analogin_t *obj) {
    return 0;
}

static inline void order(uint32_t *a, uint32_t *b) {

}

static inline uint32_t adc_read_u32(analogin_t *obj) {

    return 0;
}

uint16_t analogin_read_u16(analogin_t *obj) {
    return 0;
}

float analogin_read(analogin_t *obj) {
    return 0.0f;
}
