/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/data/data_codemorse.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_DATA_1
#include "ccodemorse.h"

namespace _ {

const CH1* ToMorseCode (CH1 code) {
  static const CH1* space = " "; //<

  static const CH1* cypher[] = {
      ".-.-.",        //< NUL, Morse code: End of message.
      0,              //< SOH.
      "-.-.-",        //< STX, Morse code: Start copying.
      "-.-..-..",     //< ETX, Morse code: Going off the air ("clear").
      "...-.-",       //< EOT, Morse code: End of transmission.
      "-.--.",        //< ENQ, Morse code: Invite a specific station to transmit.
      "...-.",        //< ACK, Morse code: Understood.
      "...---...",    //< BEL, Morse code: SOS distress signal.
      "........",     //< BS,  Morse code: Prosign error.
      space,          //< HT,  Morse code: non-standard space.
      ".-.-",         //< LF,  Morse code: New Line
      "-...-",        //< VT,  Morse code: New paragraph.
      ".-.-.",        //< FF,  Morse code: New Page
      0,              //< CR
      "-..---",       //< SO,  Morse code: Change to Wabun Mores code.
      ".--...",       //< SI,  Morse code: Non-standard return to Western Mores code
      0,              //< DLE
      0,              //< DC1
      0,              //< DC2
      0,              //< DC3
      0,              //< DC4
      0,              //< NAK
      ".-...",        //< SYN, Morse code: AS, Wait.
      0,              //< ETB
      0,              //< CAN
      "-...-.-",      //< EM,  Morse code: Break/BRB.
      0,              //< SUB
      0,              //< ESC
      0,              //< FS
      0,              //< GS
      0,              //< RS
      0,              //< US
      space,          //< ' '
      ".-..-.",       //< '!'
      ".-..-.",       //< '\"'
      0,              //< '#'
      "..._.-..",     //< '$'
      "....._..",     //< '%'
      "._...",        //< '&'
      ".----.",       //< '\''
      "-.--.-",       //< '('
      ".-..-.",       //< ')', Nonstandard, inverse of '('
      0,              //< '*'
      0,              //< '+'
      "--..--",       //< ','
      "-....-",       //< '-'
      ".-.-.-",       //< '.'
      "-..-.",        //< '/'
      "-----",        //< '0'
      ".---",         //< '1'
      "..---",        //< '2'
      "...--",        //< '3'
      "....-",        //< '4'
      ".....",        //< '5'
      "-...",         //< '6'
      "--...",        //< '7'
      "---..",        //< '8'
      "----.",        //< '9'
      "---...",       //< ':'
      0,              //< ','
      0,              //< '<'
      "-...-",        //< '='
      0,              //< '>'
      "..--..",       //< '?'
      ".--.-.",       //< '@'
      ".-",           //< 'A'
      "-...",         //< 'B'
      "-.-.",         //< 'C'
      "-..",          //< 'D'
      ".",            //< 'E'
      "..-.",         //< 'F'
      "--.",          //< 'G'
      "....",         //< 'H'
      "..",           //< 'I'
      ".---",         //< 'J'
      "-.-",          //< 'K'
      ".-..",         //< 'L'
      "--",           //< 'M'
      "-.",           //< 'N'
      "---",          //< 'O'
      ".--.",         //< 'P'
      "--.-",         //< 'Q'
      ".-.",          //< 'R'
      "...",          //< 'S'
      "-",            //< 'T'
      "..-",          //< 'u'
      "...-",         //< 'V'
      ".--",          //< 'W'
      "-..-",         //< 'X'
      "-.--",         //< 'Y'
      "--.."          //< 'Z'
  };

  if (code < 0) return 0;
  if (code >= 'a' && code <= 'z') code -= 'a' - 'A';   
  if (code > 'Z') {
    switch (code) {
      case -4: return ".-.-";   //< 132
      case -5: return ".--.-";  //< 133
      case -6: return ".--.-";  //< 134
      case -16: return "..-.."; //< 144
      case -37: return "--.--"; //< 165
      case -25: return "---.";  //< 153
      case -26: return "..--";  //< 154
      default:  return 0;
    }
  }
  return cypher[code];
}

}   //< _
#endif
