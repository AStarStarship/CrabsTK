/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cstatusled.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_STATUS_LED
#define INCLUDED_KABUKI_FEATURE_TEK_STATUS_LED 1

#define CREATE_STATUS_LED static StatusLED<0, 1> Status(GREEN_LED);

#define Assert                                                              \
  (statement, message) {                                                    \
    if (!(statement)) {                                                     \
      printf("Assert: %s\r\n%s, line %d\r\n", #message, __FILE__, __LINE__) \
          Status.HandleAssert() while (true)                                \
    }                                                                       \
  }

namespace _ {

/* Outputs the firmware status using the LED on the mbed board.
This class works by using strings with ASCII Mores Code. Each char in a
represents a pulse split into 4 lengths.
    
| Frame | ASCII | Index | Duty Cycle |
|:-----:|:-----:|:-----:|:----------:|
| Off   | ' '   | <= 44 |     0%     |
| Long  | '-'   | 45    |    1/3     |
| Short | '.'   | 46    |    1/3     |
| On    | '_'   | >= 47 |   100%     |
    
Off could be any value less than 44, and On could be any value greater than
47, but ASCII space (' ') and period ('.') are used by convention because
they look like the pulse widths.
 
## Terminology
* Frame    - Each character in a char sequence represents 3 timer
interrupts.
* Pattern  - A null-terminated string of frames.
* Sequence - A null-terminated string of const char*.
@code
StatusLED<0, 1> stausLED ();        //< Use <0, 1> if you're LED is active
low. StatusLED<1, 0> stausLED  (LED_2);   //< Use <0, 1> if you're LED is
active high.
        
const char* examplePattern[] = {
    "...   ",       //< Blinks fast three times in a row.
    "...---...   ", //< SOS in Mores Code.
    "____    ",     //< Slowly blinks on and off.
    0               //< Must have null-termination pointer.
};
statusLED.SetPattern  (exapmlePattern, 1.5f);
@endcode
*/
template <SI4 On, SI4 Off>
class StatusLED {
 public:
  static const FLT DefaultFrequency = 0.5f,  //< Default frequency in hertz.
      MinFrequency = 0.01f,                  //< Min frequency in hertz.
      MaxFrequency = 2.0f;                   //< Max frequency in hertz.

  typedef enum { Off = 0, Short = 63, Long = 127, On = 255 } Pulse;

  /* Simple constructor. */
  StatusLED(PinName led_pin = LED_1, FLT frequency = DefaultFrequency)
      : count(0),
        period(0),
        sequence(0),
        pattern(0),
        cursor(0),
        frequency(frequency),
        pin(led_pin, Off) {
    /// Nothing to do here.
  }

  /* Sets the light blinking sequence. */
  void SetSequence(char** sequence) {
    if (sequence == nullptr) {
      sequence = 0;
      StopBlinking();
      return;
    }

    const char* tempString = sequence[0];

    if (tempString == 0 || tempString[0] == 0) {
#if _Debug
      cout << "\n| Error: First sequence and first char can't be null.\n";
#endif
      return;
    }
    sequence = sequence;
    pattern = sequence[0];
    cursor = pattern;
    currentByte = *cursor;
    Update();
  }

  /* Turns off the blinker. */
  void TurnOff() { pin = Off; }

  /* Turns on the blinker. */
  void TurnOn() {
    color = colorA;
    Update();
  }

  /* Starts flashing the SOS sequence. */
  void FlashSOS() {
    sequence = SOSPattern();
    const char* _cursor = sequence[0];
    cursor = *_cursor;
    period = *_cursor;
  }

  /* Starts blinking. */
  void StartBlinking() {
    const char* _pattern = sequence[0];
    pattern = _pattern;
    cursor = _pattern;
    period = *_pattern;

    blinker.attach(this, &StatusLED::Blink, frequency / 4);
    Update();
  }

  /* Stops blinking and turns off the LED. */
  void StopBlinking() {
    TurnOff();
    blinker.detach();
    pin = Off;
    Update();
  }

  /* Sets the blink frequent. */
  void SetFrequency(FLT Value) {
    frequency = Value;
    blinker.attach(this, &StatusLED::Blink, Value);
  }

  /* Handler for the Assert macro. */
  void HandleAssert() { SetPattern(SOSPattern()); }

  /* Pattern blinks three times in a row. */
  const char** Blink3TimesPattern() {
    static const char** sequence = {"...   ", 0};
    return &sequence;
  }

  /* Standard blink sequence. */
  const char** SlowBlinkPattern() {
    static const char** sequence = {"__  ", 0};
    return &sequence;
  }

  /* Standard blink sequence. */
  const char** FastBlinkPattern() {
    static const char** sequence = {"_ ", 0};
    return &sequence;
  }

  /* Standard SOS sequence. */
  const char** SOSPattern() {
    static const char** sequence = {"...---...      ", 0};
    return &sequence;
  }

 private:
  char count,             //< Counter counts from 1-3.
      period;             //< The current period char.
  FLT frequency;          //< The period length.
  const char** sequence;  //< Null-terminated string of pointers.
  const char *pattern,    //< The current string in the sequence.
      *cursor;            //< The current char in the current string.
  DigitalOut pin;         //< Red LED on the mbed board.
  Ticker blinker;         //< Ticker for blinking the LEDs.

  /* Gets th next char in the sequence. */
  inline char GetNextPeriod() {
    /// We've already checked that the sequence and cursor and not null.

    char period_temp = *(++cursor);

    if (period_temp == 0) {
      const char* tempPattern = *(pattern + sizeof(const char*));

      if (tempPattern == nullptr) {
        const char* _cursor = sequence[0];
        cursor = pattern = _cursor;
        return *_cursor;
      }
      pattern = tempPattern;
      return *tempPattern;  //< We don't particularly care if the period is
                            //'\0'.
    }

    return period_temp;
  }

  /* Updates the status LED. */
  inline void Update() {
    const char* period_temp = period;
    if (sequence == nullptr || period_temp == nullptr) return;

    if (count == 0)  //< Beginning of cycle period.
    {
      char _period = GetNextPeriod();
      period = _period;
      count = 1;
      if (_period < '-') {
        pin = Off;
        return;
      }
      pin = On;
      return;
    } else if (count == 1)  //< 1/3 duty cycle.
    {
      count = 2;
      if (period == '.') {
        pin = Off;
        return;
      }
      return;
    }
    /// 2/3 duty cycle.
    count = 0;
    if (period > '.')  //< Leave the LED on
      return;
    pin = Off;
  }

  /* Script operations. */
  const Operation* Star(char_t index, _::Expr* expr);
};
}  // namespace _

// _D_e_m_o_____________________________________________________________________

#if 0  //< Set to non-zero to run this demo.

using namespace KabukiTek;

StatusLED Status ();
InterruptIn Switch3 (SW3);

const char* examplePattern[] = {
    "...   ",           //< Blinks fast three times in a row.
    "...---...      ",  //< SOS in Mores Code.
    "____    ",         //< Slowly blinks on and off.
    0                   //< Pattern must have null-term pointer.
};
/* Interrupt handler for SW2. */
void SwitchIRQHandler () {
    static BOL examplePatterMode = true;

    if (examplePatterMode) {
        Status.SetPattern (examplePattern);
        Status.StartBlinking ();
        examplePatterMode = false;
    } else {
        Status.SetPattern (Status.SOSPattern ()));
        examplePatterMode = true;
    }
}

SI4 main () {
    printf ("\r\n\nTesting mbed Utils.\r\n\n");
    PrintLine ();

    Switch3.rise (&SwitchIRQHandler);
    //Status.StartBlinking ()

    while (true);
}

#endif  //< Demo
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_STATUS_LED
