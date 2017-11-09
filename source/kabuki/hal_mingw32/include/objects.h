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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t port;
    uint32_t pin;
    uint32_t ch;
};

struct port_s {
    uint32_t *reg_dir;
    uint32_t *reg_out;
    uint32_t *reg_in;
    PortName port;
    uint32_t mask;
};

struct pwmout_s {
    uint32_t *MR;
    PWMName pwm;
};

struct serial_s {
    void *uart;
    int index;
};

struct analogin_s {
    ADCName adc;
};

struct dac_s {
    DACName dac;
};

struct can_s {
    void *dev;
};

struct i2c_s {
    void *i2c;
};

struct spi_s {
    void *spi;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
