/** kabuki::tek
    @file    ~/source/kabuki/tek/include/dmx_transmitter.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef KABUKI_TEK_DMX_DMXTRANSMITTER
#define KABUKI_TEK_DMX_DMXTRANSMITTER

#include "config.h"
#include "RawSerial.h"

namespace kabuki { namespace tek {


//#define DMX_UART_DIRECT

enum {
    NumDMXchannels  = 512,  //< The number of DMX512 channels.
    DMXStartCode    = 0,    //< ???
    DMXBreakTime    = 100,  //< DMX break time: 100us (88us-1s).
    DMXMABTime      = 12,   //< DMX Mark After  Break time: 12us (8us-1s).
    DMXMBBTime      = 200   //< DMX Mark Before Break time: 10us (0-1s).
};

template<int kBreakTime, int kMABTime, int kMBBTime>
int CheckTimingParameters ()
/** Function checks if the timing parameters are valid. 
*/
{
    if (kBreakTime < 88 || kBreakTime > 1000000) return -1;
    if (kMABTime   < 8  || kMABTime   > 1000000) return -1;
    if (kMBBTime   < 0  || kMBBTime   > 1000000) return -1;

    timeBreak = kBreakTime;
    timeMAB   = kMABTime;
    timeMBB   = kMBBTime;
    return 0;
}

enum DMXModes
{
    BeginState,
    DMXStartState,
    ModeState,
    MABState,
    DataState,
    DMXErrorState,
    StopState,
};

/** DMXTransmitter transmitter.
In use, the buffer size of the transmitter will not change, but the start
channel might if the device is daisy-chained. */
template<uint16_t kNumMonoLEDs, byte kNumRGBLEDs, byte kNumRGBWLEDs,
    uint16_t kBreakTime, uint16_t kMABTime, uint16_t kMBBTime>
    class DMXTransmitter {
        public:

        enum {
            kNumChannels = kNumMonoLEDs + 3 * kNumRGBLEDs + 4 * kNumRGBWLEDs
        };

        /** Simple constructor.
            @param txPin TX serial port pin.
            @param rxPin RX serial port pin.
        */
        DMXTransmitter (PinName txPin, PinName rxPin, uint16_t startChanne)
            : rawSerial (Tx, Rx) {
            clear ();
            state = BeginState;
            received_ = 0;
            sent_ = 0;
            memset (buffer, 0, sizeof (buffer));
            timeBreak = DMXBreakTime;
            timeMAB = DMXMABTime;
            timeMBB = DMXMBBTime;

#if defined (TARGET_LPC1768) || defined (TARGET_LPC2368)
            if (rxPin == P0_3) {
                serialDMX = (LPC_UART_TypeDef*)LPC_UART0;
                NVIC_SetPriority (UART0_IRQn, 1);
            } else if (rxPin == p14) {
                serialDMX = (LPC_UART_TypeDef*)LPC_UART1;
                NVIC_SetPriority (UART1_IRQn, 1);
            } else if (rxPin == p27) {
                serialDMX = LPC_UART2;
                NVIC_SetPriority (UART2_IRQn, 1);
            } else if (rxPin == p10) {
                serialDMX = LPC_UART3;
                NVIC_SetPriority (UART3_IRQn, 1);
            }
#elif defined (TARGET_LPC4088)
            if (rxPin == p10 || rxPin == P0_26 || rxPin == P4_29) {
                serialDMX = LPC_UART3;
                NVIC_SetPriority (UART3_IRQn, 1);
            } else if (rxPin == p31) {
                serialDMX = (LPC_UART_TypeDef*)LPC_UART4;
                NVIC_SetPriority (UART4_IRQn, 1);
            } else if (rxPin == P0_3) {
                serialDMX = LPC_UART0;
                NVIC_SetPriority (UART0_IRQn, 1);
            } else if (rxPin == P0_16 || rxPin == P2_1 || rxPin == P3_17) {
                serialDMX = (LPC_UART_TypeDef*)LPC_UART1;
                NVIC_SetPriority (UART1_IRQn, 1);
            } else if (rxPin == P0_11 || rxPin == P2_9 || rxPin == P4_23) {
                serialDMX = LPC_UART2;
                NVIC_SetPriority (UART2_IRQn, 1);
            }
#elif defined (TARGET_LPC11UXX)
            if (rxPin == p10) {
                serialDMX = LPC_USART;
                NVIC_SetPriority (UART_IRQn, 1);
            }
#elif defined (TARGET_LPC11XX)
            if (rxPin == P1_6) {
                serialDMX = (LPC_UART_TypeDef*)UART_0;
                NVIC_SetPriority (UART_IRQn, 1);
            }
#elif defined (TARGET_K64F)
            if (rxPin == P1_6) {
                serialDMX = (LPC_UART_TypeDef*)UART_0;
                NVIC_SetPriority (UART_IRQn, 1);
            }
#endif

            rawSerial.baud (250000);
            rawSerial.format (8, Serial::None, 2);
            rawSerial.attach (this, &DMXTransmitter::initSerial, Serial::RxIrq);

            //timeout.attach_us (this, &DMXTransmitter::InitTimer, DMX_TIME_BETWEEN);
        }

        /** Send the buffer
            @param addr DMX buffer address (0-511)
            @param buffer DMX buffer (0-255) */
        void Setchannel (uint16_t channel, byte Value) {
            if (channel < startchanne || channel > NumDMXchannels) return;
            buffer[channel - startchanne] = Value;
        }

        /** Send the buffer
            @param Buffer DMX buffer
            @param addr DMX buffer address
            @param Length buffer length */
        void Write (byte *Buffer, uint16_t startchanne = 0, int Length = NumDMXchannels) {
            if (channel < 0 || channel >= NumDMXchannels) return;
            if (Length > NumDMXchannels - channel) Length = NumDMXchannels - channel;
            memcpy (&buffer[channel], Buffer, Length)
        }

        /** Send the buffer
            @param addr DMX buffer address (0-511)
            @return DMX buffer (0-255) */
        byte Get (uint16_t channel) {
            if (channel < 0 || channel >= NumDMXchannels) return 0;
            return buffer[channel];
        }

        /** Start DMX send operation */
        void start ()
        {
            if (state == StopState) {
                state = BeginState;
                sent_ = 0;
                timeout.attach_us (this, &DMXTransmitter::InitTimer, timeMBB);
            }
        }

        /** Stop DMX send operation */
        void Stop () {
            rawSerial.attach (0, Serial::TxIrq);
            timeout.detach ();
            state = StopState;
        }

        /** Clears the DMX buffer */
        void Clear () {
            for (int i = 0; i < NumDMXchannels; i++)
                buffer[i] = 0;
        }

        /** Marks the DMX buffer as being received. */
        int MarkReceived () {
            int r = received_;
            received_ = 0;
            return r;
        }

        /** Marks the DMX buffer as being sent. */
        int MarkSent () {
            int r = sent_;
            sent_ = 0;
            return r;
        }

        /** Gets a pointer to the first byte in the buffer. */
        byte* GetBuffer () {
            return buffer;
        }

        private:

        RawSerial rawSerial;            //< Raw serial port.

        Timeout timeout;                //< The timeout timer.

        volatile DMXMode mode;          //< The current mode.

        volatile int startChannel,      //< The start DMX channel.
            numchannels,                //< The number of DMX channels.
            index;                      //< The index in the 512 DMX channels.

        int isDone,                     //< Flag for if the Rx or Tx process is done.
            timeBreak,                  //< Break time?
            timeMAB,                    //< Mark-after-break time.
            timeMBB;                    //< Mark-before-break time.

        byte buffer[kNumChannels];       //< Pointer to the buffer.

#if defined (TARGET_LPC1768) || defined (TARGET_LPC2368) || defined (TARGET_LPC4088)
        LPC_UART_TypeDef* serialDMX;
#elif defined (TARGET_LPC11UXX)
        LPC_USART_Type* serialDMX;
#elif defined (TARGET_LPC11XX)
        LPC_UART_TypeDef* serialDMX;
#elif 
#else
#error "CPU not supported."
#endif

        void InitTimer ()           //< initializes the timer.
        {

            switch (state) {
                case BeginState:
                    // Break Time
                    timeout.detach ();
                    serialDMX->LCR |= (1 << 6);
                    state = ModeState;
                    timeout.attach_us (this, &DMXTransmitter::InitTimer, timeBreak);
                    break;
                case ModeState:
                    // Mark After Break
                    timeout.detach ();
                    serialDMX->LCR &= ~(1 << 6);
                    state = MABState;
                    timeout.attach_us (this, &DMXTransmitter::InitTimer, timeMAB);
                    break;
                case MABState:
                    // Start code
                    timeout.detach ();
                    channel_tx = 0;
                    state = DataState;
                    rawSerial.attach (this, &DMXTransmitter::init, Serial::TxIrq);
#ifdef DMX_UART_DIRECT
                    while (!(serialDMX->LSR & (1 << 5)));
                    serialDMX->THR = DMX_START_CODE;
#else
                    rawSerial.putc (DMX_START_CODE);
#endif
                    break;
            }
        }

        void initSerial () {
            // Data
            if (state == DataState) {
                if (channel_tx < NumDMXchannels) {
#ifdef DMX_UART_DIRECT
                    serialDMX->THR = (byte)buffer[channel_tx];
#else
                    rawSerial.putc (buffer[channel_tx])
#endif
                        channel_tx++;
                } else {
                    rawSerial.attach (0, Serial::TxIrq)
                        state = BeginState;
                    sent_ = 1;
                    timeout.attach_us (this, &DMXTransmitter::InitTimer, timeMBB)
                }
            }
        }
};
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_DMX_DMXTRANSMITTER
