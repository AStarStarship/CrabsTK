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

#include "../../hal/include/ethernet_api.h"
#include "../../hal/include/mbed_interface.h"
#include "../../hal/include/toolchain.h"
#include "../../hal/include/mbed_error.h"

#if NEW_LOGIC
static int rx_consume_offset = -1;
static int tx_produce_offset = -1;
#else
static int send_doff =  0;
static int send_idx  = -1;
static int send_size =  0;

static int receive_soff =  0;
static int receive_idx  = -1;
#endif

static uint32_t phy_id = 0;

static inline int rinc(int idx, int mod) {
    return 0;
}

//extern unsigned int SystemFrequency;
static inline unsigned int clockselect() {
    return 0;
}

#ifndef min
#define min(x, y) (((x)<(y))?(x):(y))
#endif

/*----------------------------------------------------------------------------
  Ethernet Device initialize
 *----------------------------------------------------------------------------*/
int ethernet_init() {
    return -1;
}

/*----------------------------------------------------------------------------
  Ethernet Device Uninitialize
 *----------------------------------------------------------------------------*/
void ethernet_free() {
}

// if(TxProduceIndex == TxConsumeIndex) buffer array is empty
// if(TxProduceIndex == TxConsumeIndex - 1) buffer is full, should not fill
// TxProduceIndex - The buffer that will/is being fileld by driver, s/w increment
// TxConsumeIndex - The buffer that will/is beign sent by hardware

int ethernet_write(const char *data, int slen) {
    return -1;
}

int ethernet_send() {
    return -1;
}

// RxConsmeIndex - The index of buffer the driver will/is reading from. Driver should inc once read
// RxProduceIndex - The index of buffer that will/is being filled by MAC. H/w will inc once rxd
//
// if(RxConsumeIndex == RxProduceIndex) buffer array is empty
// if(RxConsumeIndex == RxProduceIndex + 1) buffer array is full

// Recevies an arrived ethernet packet.
// Receiving an ethernet packet will drop the last received ethernet packet
// and make a new ethernet packet ready to read.
// Returns size of packet, else 0 if nothing to receive

// We read from RxConsumeIndex from position rx_consume_offset
// if rx_consume_offset < 0, then we have not recieved the RxConsumeIndex packet for reading
// rx_consume_offset = -1 // no frame
// rx_consume_offset = 0  // start of frame
// Assumption: A fragment should alway be a whole frame

int ethernet_receive() {
    return -1;
}

// Read from an recevied ethernet packet.
// After receive returnd a number bigger than 0 it is
// possible to read bytes from this packet.
// Read will write up to size bytes into data.
// It is possible to use read multible times.
// Each time read will start reading after the last read byte before.

int ethernet_read(char *data, int dlen) {
    return -1;
}

int ethernet_link(void) {
    return -1;
}

static int phy_write(unsigned int PhyReg, unsigned short Data) {
    return -1;
}

static int phy_read(unsigned int PhyReg) {
    return -1;
}


static void txdscr_init() {
}

static void rxdscr_init() {
}

void ethernet_address(char *mac) {
}

void ethernet_set_link(int speed, int duplex) {
}
