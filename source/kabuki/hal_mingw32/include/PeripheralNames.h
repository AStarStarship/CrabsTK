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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = 0,
    UART_1 = 0,
    UART_2 = 0,
    UART_3 = 0
} UARTName;

typedef enum {
    ADC0_0 = 0,
    ADC0_1,
    ADC0_2,
    ADC0_3,
    ADC0_4,
    ADC0_5,
    ADC0_6,
    ADC0_7
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;

typedef enum {
    SPI_0 = 0,
    SPI_1 = 0
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1 = 0,
    I2C_2 = 0
} I2CName;

typedef enum {
    PWM_1 = 1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6
} PWMName;

typedef enum {
     CAN_1 = 0,
     CAN_2 = 0
} CANName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_2

// Default peripherals
#define MBED_SPI0         p5, p6, p7, p8
//#define MBED_SPI1         p11, p12, p13, p14

#define MBED_UART0        p9, p10
#define MBED_UART1        p13, p14
#define MBED_UART2        p15, p16
#define MBED_UARTUSB      USBTX, USBRX

#define MBED_I2C0         p17, p18
//#define MBED_I2C1         p9, p10

#define MBED_CAN0         p19, p20

#define MBED_ANALOGOUT0   p21

#define MBED_ANALOGIN0    p22
#define MBED_ANALOGIN1    p23
//#define MBED_ANALOGIN2    p17
//#define MBED_ANALOGIN3    p18
//#define MBED_ANALOGIN4    p19
//#define MBED_ANALOGIN5    p20

#define MBED_PWMOUT0      p24
#define MBED_PWMOUT1      p25
#define MBED_PWMOUT2      p26
#define MBED_PWMOUT3      p27
//#define MBED_PWMOUT4      p22
//#define MBED_PWMOUT5      p21

#define MBED_USB_D_PLUS   p28
#define MBED_USB_D_MINUS  p29

#define MBED_MCICLK       p30
#define MBED_MCICMD       p31
#define MBED_MCIDAT0      p32
#define MBED_MCIDAT1      p33
#define MBED_MCIDAT2      p34
#define MBED_MCIDAT3      p35

#ifdef __cplusplus
}
#endif

#endif
