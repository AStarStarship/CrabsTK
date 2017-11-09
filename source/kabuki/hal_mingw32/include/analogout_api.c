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
#include "../../hal/include/analogout_api.h"
#include "../../hal/include/pinmap.h"

static const PinMap PinMap_DAC[] = {
    0,0
};

void analogout_init(dac_t *obj, PinName pin) {
}

void analogout_free(dac_t *obj) {}

static inline void dac_write(int value) {
}

static inline int dac_read() {
    return 0;
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(0);
    } else if (value > 1.0f) {
        dac_write(0x3FF);
    } else {
        dac_write(value * (float)0x3FF);
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    dac_write(value >> 6); // 10-bit
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read();
    return (float)value * (1.0f / (float)0x3FF);
}

uint16_t analogout_read_u16(dac_t *obj) {
    uint32_t value = dac_read(); // 10-bit
    return (value << 6) | ((value >> 4) & 0x003F);
}
