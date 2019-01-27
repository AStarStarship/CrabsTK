/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git */

#pragma once
#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_DMX_DMXTRANSMITTER
#define INCLUDED_KABUKI_FEATURE_TEK_DMX_DMXTRANSMITTER

#include "RawSerial.h"

namespace _ {

//#define DMX_UART_DIRECT

enum {
  kDMXChannelCount = 512,  //< The number of DMX512 channels.
  kDMXStartCode = 0,     //< ???
  kDMXTimeBreak = 100,    //< DMX break time: 100us (88us-1s).
  kDMXTimeMAB = 12,       //< DMX Mark After  Break time: 12us (8us-1s).
  DMXTimeMBB = 200       //< DMX Mark Before Break time: 10us (0-1s).
};

template <SI4 kBreakTime, SI4 kMABTime, SI4 kMBBTime>
SI4 CheckTimingParameters()
/* Function checks if the timing parameters are valid.
 */
{
  if (kBreakTime < 88 || kBreakTime > 1000000) return -1;
  if (kMABTime < 8 || kMABTime > 1000000) return -1;
  if (kMBBTime < 0 || kMBBTime > 1000000) return -1;

  timeBreak = kBreakTime;
  timeMAB = kMABTime;
  timeMBB = kMBBTime;
  return 0;
}

enum DMXModes {
  kStateBegin,
  kStateDMXStart,
  kStateMode,
  kStateMAB,
  kStateData,
  DMXErrorState,
  StopState,
};

/* DMXTransmitter transmitter.
In use, the buffer size of the transmitter will not change, but the start
channel might if the device is daisy-chained. */
template <UI2 kNumMonoLEDs, UI1 kNumRGBLEDs, UI1 kNumRGBWLEDs, UI2 kBreakTime,
          UI2 kMABTime, UI2 kMBBTime>
class DMXTransmitter {
 public:
  enum { kChannelCount = kNumMonoLEDs + 3 * kNumRGBLEDs + 4 * kNumRGBWLEDs };

  /* Simple constructor.
      @param txPin TX serial port pin.
      @param rxPin RX serial port pin.
  */
  DMXTransmitter(PinName txPin, PinName rxPin, UI2 startChanne)
      : rawSerial(Tx, Rx) {
    clear();
    state = kStateBegin;
    received_ = 0;
    sent_ = 0;
    memset(buffer, 0, sizeof(buffer));
    timeBreak = kDMXTimeBreak;
    timeMAB = kDMXTimeMAB;
    timeMBB = DMXTimeMBB;

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    if (rxPin == P0_3) {
      serialDMX = (LPC_UART_TypeDef*)LPC_UART0;
      NVIC_SetPriority(UART0_IRQn, 1);
    } else if (rxPin == p14) {
      serialDMX = (LPC_UART_TypeDef*)LPC_UART1;
      NVIC_SetPriority(UART1_IRQn, 1);
    } else if (rxPin == p27) {
      serialDMX = LPC_UART2;
      NVIC_SetPriority(UART2_IRQn, 1);
    } else if (rxPin == p10) {
      serialDMX = LPC_UART3;
      NVIC_SetPriority(UART3_IRQn, 1);
    }
#elif defined(TARGET_LPC4088)
    if (rxPin == p10 || rxPin == P0_26 || rxPin == P4_29) {
      serialDMX = LPC_UART3;
      NVIC_SetPriority(UART3_IRQn, 1);
    } else if (rxPin == p31) {
      serialDMX = (LPC_UART_TypeDef*)LPC_UART4;
      NVIC_SetPriority(UART4_IRQn, 1);
    } else if (rxPin == P0_3) {
      serialDMX = LPC_UART0;
      NVIC_SetPriority(UART0_IRQn, 1);
    } else if (rxPin == P0_16 || rxPin == P2_1 || rxPin == P3_17) {
      serialDMX = (LPC_UART_TypeDef*)LPC_UART1;
      NVIC_SetPriority(UART1_IRQn, 1);
    } else if (rxPin == P0_11 || rxPin == P2_9 || rxPin == P4_23) {
      serialDMX = LPC_UART2;
      NVIC_SetPriority(UART2_IRQn, 1);
    }
#elif defined(TARGET_LPC11UXX)
    if (rxPin == p10) {
      serialDMX = LPC_USART;
      NVIC_SetPriority(UART_IRQn, 1);
    }
#elif defined(TARGET_LPC11XX)
    if (rxPin == P1_6) {
      serialDMX = (LPC_UART_TypeDef*)UART_0;
      NVIC_SetPriority(UART_IRQn, 1);
    }
#elif defined(TARGET_K64F)
    if (rxPin == P1_6) {
      serialDMX = (LPC_UART_TypeDef*)UART_0;
      NVIC_SetPriority(UART_IRQn, 1);
    }
#endif

    rawSerial.baud(250000);
    rawSerial.format(8, Serial::None, 2);
    rawSerial.attach(this, &DMXTransmitter::initSerial, Serial::RxIrq);

    // timeout.attach_us (this, &DMXTransmitter::InitTimer, DMX_TIME_BETWEEN);
  }

  /* Send the buffer
      @param addr DMX buffer address (0-511)
      @param buffer DMX buffer (0-255) */
  void Setchannel(UI2 channel, UI1 Value) {
    if (channel < startchanne || channel > kDMXChannelCount) return;
    buffer[channel - startchanne] = Value;
  }

  /* Send the buffer
      @param Buffer DMX buffer
      @param addr DMX buffer address
      @param Length buffer length */
  void Write(UI1* Buffer, UI2 startchanne = 0, SI4 Length = kDMXChannelCount) {
    if (channel < 0 || channel >= kDMXChannelCount) return;
    if (Length > kDMXChannelCount - channel) Length = kDMXChannelCount - channel;
    memcpy(&buffer[channel], Buffer, Length)
  }

  /* Send the buffer
      @param addr DMX buffer address (0-511)
      @return DMX buffer (0-255) */
  UI1 Get(UI2 channel) {
    if (channel < 0 || channel >= kDMXChannelCount) return 0;
    return buffer[channel];
  }

  /* Start DMX send operation */
  void start() {
    if (state == StopState) {
      state = kStateBegin;
      sent_ = 0;
      timeout.attach_us(this, &DMXTransmitter::InitTimer, timeMBB);
    }
  }

  /* Stop DMX send operation */
  void Stop() {
    rawSerial.attach(0, Serial::TxIrq);
    timeout.detach();
    state = StopState;
  }

  /* Clears the DMX buffer */
  void Clear() {
    for (SI4 i = 0; i < kDMXChannelCount; i++) buffer[i] = 0;
  }

  /* Marks the DMX buffer as being received. */
  SI4 MarkReceived() {
    SI4 r = received_;
    received_ = 0;
    return r;
  }

  /* Marks the DMX buffer as being sent. */
  SI4 MarkSent() {
    SI4 r = sent_;
    sent_ = 0;
    return r;
  }

  /* Gets a pointer to the first UI1 in the buffer. */
  UI1* GetBuffer() { return buffer; }

 private:
  RawSerial rawSerial;  //< Raw serial port.

  Timeout timeout;  //< The timeout timer.

  volatile DMXMode mode;  //< The current mode.

  volatile SI4 startChannel,  //< The start DMX channel.
      numchannels,            //< The number of DMX channels.
      index;                  //< The index in the 512 DMX channels.

  SI4 isDone,     //< Flag for if the Rx or Tx process is done.
      timeBreak,  //< Break time?
      timeMAB,    //< Mark-after-break time.
      timeMBB;    //< Mark-before-break time.

  UI1 buffer[kChannelCount];  //< Pointer to the buffer.

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || \
    defined(TARGET_LPC4088)
  LPC_UART_TypeDef* serialDMX;
#elif defined(TARGET_LPC11UXX)
  LPC_USART_Type* serialDMX;
#elif defined(TARGET_LPC11XX)
  LPC_UART_TypeDef* serialDMX;
#elif
#else
#error "CPU not supported."
#endif

  void InitTimer()  //< initializes the timer.
  {
    switch (state) {
      case kStateBegin:
        // Break Time
        timeout.detach();
        serialDMX->LCR |= (1 << 6);
        state = kStateMode;
        timeout.attach_us(this, &DMXTransmitter::InitTimer, timeBreak);
        break;
      case kStateMode:
        // Mark After Break
        timeout.detach();
        serialDMX->LCR &= ~(1 << 6);
        state = kStateMAB;
        timeout.attach_us(this, &DMXTransmitter::InitTimer, timeMAB);
        break;
      case kStateMAB:
        // Start code
        timeout.detach();
        channel_tx = 0;
        state = kStateData;
        rawSerial.attach(this, &DMXTransmitter::init, Serial::TxIrq);
#ifdef DMX_UART_DIRECT
        while (!(serialDMX->LSR & (1 << 5)))
          ;
        serialDMX->THR = DMX_START_CODE;
#else
        rawSerial.putc(DMX_START_CODE);
#endif
        break;
    }
  }

  void initSerial() {
    // Data
    if (state == kStateData) {
      if (channel_tx < kDMXChannelCount) {
#ifdef DMX_UART_DIRECT
        serialDMX->THR = (UI1)buffer[channel_tx];
#else
        rawSerial.putc(buffer[channel_tx])
#endif
        channel_tx++;
      } else {
        rawSerial.attach(0, Serial::TxIrq) state = kStateBegin;
        sent_ = 1;
        timeout.attach_us(this, &DMXTransmitter::InitTimer, timeMBB)
      }
    }
  }
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_DMX_DMXTRANSMITTER
