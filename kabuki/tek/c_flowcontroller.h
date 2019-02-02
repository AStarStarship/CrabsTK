/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/tek/cflowcontroller.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#ifndef INCLUDED_KABUKI_FEATURE_TEK_FLOW_CONTROLLER
#define INCLUDED_KABUKI_FEATURE_TEK_FLOW_CONTROLLER 1

namespace _ {

/* Converts gallows to milliliters. */
UI4 ConvertGallonsToMilliiters(FP4 value);

/* Converts milliliters to gallons. */
FP4 ConvertMilliitersToGallons(UI4 value);

class FlowController {
 public:
  /* Constructs a smart waterer. */
  FlowController(PinName sensor_pin, PinName solenoid_pin, PinName pot_pin,
                 UI4 max_flow_ml);

  /* StartWaterings to the beginning of the watering cycle. */
  void StartWatering(SI4 index);

  /* Increments the flow rate sensor pulse counter. */
  void PulseFlowSensor();

  /* Prints the state of object to the debug stream. */
  void Print(SI4 index);

  /* Prints the state of object when done watering to the debug stream. */
  inline void StopWatering(SI4 index);

  /* Polls the pot and updates the target flow. */
  void UpdateTargetFlow();

  /* Updates the FP4 rate. */
  void Update(SI4 index);

  /* Opens the solenoid valve. */
  void OpenValve();

  /* Closes the solenoid valve. */
  void CloseValve();

  /* Script operations. */
  const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  InterruptIn sensor_;      //< Flow sensor pin.
  DigitalOut valve_;        //< Solenoid valve pin.
  AnalogIn pot_;            //< Potentiometer pin.
  volatile UI2 count_;      //< Flow sensor pulse count_.
  UI2 flow_rate_ml_;        //< Flow rate in milliliters.
  UI4 total_flow_ml_,  //< Total flow in mL.
      target_flow_ml_,      //< Target flow in mL per cycle.
      max_flow_ml_;         //< Maximum target flow in mL per cycle.
  UI2 last_sample_;         //< Last pot value read.
};

class FlowControllerOp : public _::Operand {
 public:
  /* Constructs a smart waterer. */
  FlowControllerOp(FlowController* flow_co);

  /* Script operations. */
  virtual const _::Operation* Star(char_t index, _::Expr* expr);

 private:
  FlowController* flow_co_;  //< The FlowController.
};
}  // namespace _
#endif  //< INCLUDED_KABUKI_FEATURE_TEK_FLOW_CONTROLLER
