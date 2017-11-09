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
#include "../../hal/include/port_api.h"
#include "../../hal/include/pinmap.h"
#include "../../hal/include/gpio_api.h"

PinName port_pin(PortName port, int pin_n) {
    return 0;
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir) {
}

void port_mode(port_t *obj, PinMode mode) {
}

void port_dir(port_t *obj, PinDirection dir) {
    switch (dir) {
        case PIN_INPUT : *obj->reg_dir &= ~obj->mask; break;
        case PIN_OUTPUT: *obj->reg_dir |=  obj->mask; break;
    }
}

void port_write(port_t *obj, int value) {
    *obj->reg_out = (*obj->reg_in & ~obj->mask) | (value & obj->mask);
}

int port_read(port_t *obj) {
    return (*obj->reg_in & obj->mask);
}
