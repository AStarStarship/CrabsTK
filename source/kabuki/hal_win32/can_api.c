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

#include "../../hal/include/mbed_assert.h"
#include "../../hal/include/can_api.h"
#include "../../hal/include/pinmap.h"

#include <math.h>
#include <string.h>

/* Acceptance filter mode in AFMR register */
#define ACCF_OFF                0x01
#define ACCF_BYPASS             0x02
#define ACCF_ON                 0x00
#define ACCF_FULLCAN            0x04

/* There are several bit timing calculators on the internet.
http://www.port.de/engl/canprod/sv_req_form.html
http://www.kvaser.com/can/index.htm
*/

static const PinMap PinMap_CAN_RD[] = {
    {P0_0 , CAN_1, 1},
    {P0_4 , CAN_2, 2},
    {P0_21, CAN_1, 3},
    {P2_7 , CAN_2, 1},
    {NC   , NC   , 0}
};

static const PinMap PinMap_CAN_TD[] = {
    {P0_1 , CAN_1, 1},
    {P0_5 , CAN_2, 2},
    {P0_22, CAN_1, 3},
    {P2_8 , CAN_2, 1},
    {NC   , NC   , 0}
};

// Type definition to hold a CAN message
struct CANMsg {
    unsigned int  reserved1 : 16;
    unsigned int  dlc       :  4; // Bits 16..19: DLC - Data Length Counter
    unsigned int  reserved0 : 10;
    unsigned int  rtr       :  1; // Bit 30: Set if this is a RTR message
    unsigned int  type      :  1; // Bit 31: Set if this is a 29-bit ID message
    unsigned int  id;             // CAN Message ID (11-bit or 29-bit)
    unsigned char data[8];        // CAN Message Data Bytes 0-7
};
typedef struct CANMsg CANMsg;

static uint32_t can_disable(can_t *obj) {
    uint32_t sm = obj->dev->MOD;
    obj->dev->MOD |= 1;
    return sm;
}

static inline void can_enable(can_t *obj) {
    if (obj->dev->MOD & 1) {
        obj->dev->MOD &= ~(1);
    }
}

int can_mode(can_t *obj, CanMode mode) {
    return 0; // not implemented
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle) {
    return 0; // not implemented
}

static int can_pclk(can_t *obj) {
    return 0;
}

// This table has the sampling points as close to 75% as possible. The first
// value is TSEG1, the second TSEG2.
static const int timing_pts[23][2] = {
    {0x0, 0x0},      // 2,  50%
    {0x1, 0x0},      // 3,  67%
    {0x2, 0x0},      // 4,  75%
    {0x3, 0x0},      // 5,  80%
    {0x3, 0x1},      // 6,  67%
    {0x4, 0x1},      // 7,  71%
    {0x5, 0x1},      // 8,  75%
    {0x6, 0x1},      // 9,  78%
    {0x6, 0x2},      // 10, 70%
    {0x7, 0x2},      // 11, 73%
    {0x8, 0x2},      // 12, 75%
    {0x9, 0x2},      // 13, 77%
    {0x9, 0x3},      // 14, 71%
    {0xA, 0x3},      // 15, 73%
    {0xB, 0x3},      // 16, 75%
    {0xC, 0x3},      // 17, 76%
    {0xD, 0x3},      // 18, 78%
    {0xD, 0x4},      // 19, 74%
    {0xE, 0x4},      // 20, 75%
    {0xF, 0x4},      // 21, 76%
    {0xF, 0x5},      // 22, 73%
    {0xF, 0x6},      // 23, 70%
    {0xF, 0x7},      // 24, 67%
};

static unsigned int can_speed(unsigned int sclk, unsigned int pclk, unsigned int cclk, unsigned char psjw) {
    return 0;
}

void can_init(can_t *obj, PinName rd, PinName td) {
}

void can_free(can_t *obj) {
}

int can_frequency(can_t *obj, int f) {
    return 0;
}

int can_write(can_t *obj, CAN_Message msg, int cc) {
    return 0;
}

int can_read(can_t *obj, CAN_Message *msg, int handle) {
    return 0;
}

void can_reset(can_t *obj) {
    can_disable(obj);
    obj->dev->GSR = 0; // Reset error counter when CAN1MOD is in reset
}

unsigned char can_rderror(can_t *obj) {
    return (obj->dev->GSR >> 16) & 0xFF;
}

unsigned char can_tderror(can_t *obj) {
    return (obj->dev->GSR >> 24) & 0xFF;
}

void can_monitor(can_t *obj, int silent) {
    uint32_t mod_mask = can_disable(obj);
    if (silent) {
        obj->dev->MOD |= (1 << 1);
    } else {
        obj->dev->MOD &= ~(1 << 1);
    }
    if (!(mod_mask & 1)) {
        can_enable(obj);
    }
}
