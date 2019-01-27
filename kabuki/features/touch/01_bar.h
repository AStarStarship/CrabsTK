/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/01_bar.h
@author  Cale McCollough <calemccollough.github.io>
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

#if SEAM == KABUKI_FEATURES_TOUCH_0
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

using namespace _;

namespace kabuki { namespace toolkit { namespace touch {
inline const CH1* _1_Bar (CH1* seam_log, CH1* seam_end, const CH1* args) {
#if SEAM >= KABUKI_FEATURES_TOUCH_0
  TEST_BEGIN;
  control_array a();

  PRINTF("Testing const CH1* print (_::Expr& slot)...");
  a.Print();

  DMXControl c1("Control 1", 50, 33);
  MIDIControl c2("Control 2", 11, 102);

  PRINTF("testing void setControl  (Control*)");

  a.SetControl(0, &c1);
  a.SetControl(1, &c2);

  PRINTF("Added 2 controls to a:\n");
  a.Print();

  DMXControl c3("Control 3", 27, 66);
  MIDIControl c4("Control 4", 62, 24);

  a.AddControl(&c3);
  a.AddControl(&c4);

  PRINTF("Attempted to add 2 more controls a:\n");
  a.Print();

  DMXButton c5("Control 5", 27, 66);
  MIDIButton c6("Control 6", 62, 24);
  MacroButton c7("Control 7");
  c7.Add(&c1);
  c7.Add(&c2);
  c7.Add(&c3);
  c7.Add(&c4);
  c7.Add(&c5);
  c7.Add(&c6);

  a.AddControl(&c5);
  a.AddControl(&c6);
  a.AddControl(&c7);

  PRINT("Attempted to add some buttons:\n");
  a.Print();

  PRINTF("_::control_array tests completed successfully. :-)");

  PRINTF("_::DMXButton class SDK\n");

  DMXButton a("DMX Button A");

  PRINTF("Testing const CH1* print (_::Expr& slot)...");
  a.Print();

  PRINTF("Testing void processPress () and void processDepress ()");
  a.Press();
  CHECK(a.isPressed());
  a.Depress();
  CHECK(!a.isPressed());

  PRINTF("Testing void toggle ()");
  a.SetButtonAction(ButtonAction::Latching);
  a.Toggle();
  CHECK(a.GetValue() == a.GetMaxValue());
  a.Toggle();
  CHECK(a.GetValue() == a.GetMinValue());

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

  PRINTF("Testing ::_::ControlMatrix class.");

  PRINTF("Testing const CH1* print (_::Expr& slot):\n");
  ControlMatrix cm(5, 2);
  cm.Print();

  PRINTF("Testing setControl ()");

  DMXControl testControl0_0("DMX Control 1");
  MIDIControl testControl1_0("MIDI Control 1");
  DMXButton testControl2_0("DMX Button 1");
  MIDIButton testControl3_0("MIDI Button 1");
  MacroButton testControl4_0("Macro Button 1");

  DMXControl testControl0_1("DMX Control 2");
  MIDIControl testControl1_1("MIDI Control 2");
  DMXButton testControl2_1("DMX Button 2");
  MIDIButton testControl3_1("MIDI Button 2");
  MacroButton testControl4_1("Macro Button 2");

  cm.SetControl(0, 0, &testControl0_0);
  cm.SetControl(1, 0, &testControl1_0);
  cm.SetControl(2, 0, &testControl2_0);
  cm.SetControl(3, 0, &testControl3_0);
  cm.SetControl(4, 0, &testControl4_0);

  cm.SetControl(0, 1, &testControl0_1);
  cm.SetControl(1, 1, &testControl1_1);
  cm.SetControl(2, 1, &testControl2_1);
  cm.SetControl(3, 1, &testControl3_1);
  cm.SetControl(4, 1, &testControl4_1);

  cm.Print();

  PRINTF("Done testing ::_::ControlMatrix class SDK! :-)");

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

  PRINTF("Running Widget tests...");

  PRINTF("Testing ::_::Widget class SDK\n");
  PRINT_LINE('~');

  Widget w("Test Widget");

  PRINTF("Testing const CH1* ToString ()\n");

  w.Print();

  PRINTF("Testing InsertPage  (const CH1*&)\n");

  const CH1* page_name_string;
  CH1 pageIndex = '1';

  for (SI4 i = 0; i < 6; ++i) {
    page_name_string = "Page " + pageIndex;
    PRINTF("Inserting Page %s...", page_name_string);
    CHECK(!w.InsertPage(page_name_string),
          "Error: Failed to insert page " + page_name_string + "!\n");
    ++pageIndex;
  }

  PRINTF(w.ToString());

  PRINTF("Testing contains  (const CH1*&)...");

  CHECK(!w.contains("Page 1"), "Error: w does not contains \"Page 1\"");

  PRINTF("Testing deletePage  (SI4)... ");

  SI4 debug_code;

  debug_code = w.deletePage(1);

  CHECK(debug_code == 1,
        "Error: w.deletePage  (1) returned invalid debug_code: " + debug_code);

  w.deletePage(2);
  w.deletePage(3);
  debug_code = w.deletePage(3);
  CHECK(debug_code == 1, "Error: debugCdode was not equal to 1: " + debug_code);
  w.deletePage(3);
  w.deletePage(1);
  w.deletePage(2);
  w.deletePage(2);
  w.deletePage(2);
  w.deletePage(1);
  w.deletePage(1);
  w.deletePage(1);
  w.Print();

  PRINTF("Testing inserting duplicate page names...");

  for (SI4 i = 0; i < 5; ++i) {
    debug_code = w.InsertPage("Untitled");
    PRINTF((i + 1) + ".) Debug code: " + debug_code);
  }
  debug_code = w.InsertPage("Untitled");

  w.deletePage("Untitled 4");
  debug_code = w.InsertPage("Untitled");

  PRINTF(w.ToString());

  PRINTF("Testing copy constructor...");
  Widget wc(w);
  wc.Print();

  PRINTF("Done testing Widget class SDK");

  PRINTF("\n\nRunning ::_::Widget tests completed.\n\n");

  PRINTF("Testing ::_::WidgetPage");

  PRINTF("Testing Page class\n");
  PRINT_LINE('~');

  WidgetPage p("Test Page");

  PRINTF("Testing const CH1* toString ()");
  p.Print();

  PRINTF("Testing copy constructor...");
  WidgetPage pc(p);
  pc.Print();

  PRINTF("Done testing Page class");

  PRINTF("\n\nRunning ::_::WidgetPage tests completed.\n\n");

  PRINTF("Testing ::_::MIDIControl\n");

  MIDIControl a("MIDI Control A");
  a.Print();

  PRINTF("Testing setCC ()");
  a.SetCC(127);
  CHECK(a.GetCC() == 127);
  a.SetCC(128);
  CHECK(a.GetCC() == 127);
  a.SetCC(-1);
  PRINTF("a:\n");
  a.Print();
  CHECK(a.GetCC() == 0);

  PRINTF("Done testing ::_::MIDIControl class SDK. :-)\n");

  PRINTF("_::MIDIButton...\n");

  MIDIButton a("MIDI Button A");

  PRINTF("Testing const CH1* print (_::Expr& slot)");
  a.Print();

  PRINTF("\n\n_Theater::MIDIButton tests completed.\n\n");

  PRINTF("Testing ::_::MacroButton class SDK:\n");

  auto macroA = MacroButton("Macro Button A");

  PRINTF("Testing const CH1* print (_::verifier&)...");
  PRINTF(macroA.Print(_::Expr & slot));
  PRINTF("Testing add  (Parameter<SI4>*)");
  macroA.add(new DMXControl("Macro 1"));
  macroA.add(new MIDIControl("Macro 2"));
  macroA.add(new DMXButton("Macro 3"));
  macroA.add(new MIDIButton("Macro 4"));
  PRINTF("Added " + const CH1*(macroA.numControls()) + " test controls.\n" +
         macroA.Print(_::Expr & slot));
  PRINTF("Testing void trigger ()");
  macroA.trigger();
  PRINTF("Testing Parameter<SI4>* remove  (index);");

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

  PRINTF("_::Label Class.\n");

  const CH1 *invalid_string = "Invalid label ~",
             *valid_string_1 = "Valid label 1",
             *valid_string_2 = "Valid label !@%^",
             *valid_string_3 = "Valid label &* ()";

  Label valid_label_1(valid_string_1);
  Label valid_label_2(valid_string_1);
  Label valid_label_3(valid_string_3);
  Label invalid_label(invalid_string);

  PRINTF("Testing valid string input.");

  PRINTF("Testing SI4 isValid  (const CH1* &)");
  PRINTF("isValidString(valid_string_1): %b", is_valid_label(valid_string_1));
  CHECK(is_valid_label(valid_string_1) == Valid);
  PRINTF("Testing invalid string input.");
  CHECK(is_valid_label(invalid_string) != Valid);
  CHECK(is_valid_label("") != 0);
  CHECK(is_valid_label("123456789012345678901") != 0);

  PRINTF("Testing SI4 compare  (const CH1* &)\n");
  invalid_label.Print();
  PRINTF("Testing valid comparisons...");
  CHECK(!valid_label_1.Compare(valid_string_1));
  PRINTF("Testing invalid comparisons...");
  CHECK(valid_label_1.Compare(valid_string_2) != Comperable);
  CHECK(valid_label_2.Compare(valid_string_3) != Comperable);

  PRINTF("Attempting to SetLabel to the invalid_string");
  // auto expectedLabel = "Test string 1";
  valid_label_1.SetLabel(invalid_string);
  CHECK(!valid_label_1.Compare(invalid_label));
  PRINTF("Setting the label to valid_string_3");
  valid_label_1.SetLabel(valid_string_3);
  CHECK(valid_label_1.GetLabel() == valid_string_3);

  PRINTF("Done testing ::_::Label class SDK.");

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

  PRINTF("Testing IsymmetricController class _KabukiToolkit\n");
  PRINT_LINE('~');

  IsymmetricController controller();

  PRINTF("Testing ToString ().");
  PRINTF(controller.Print());

  // testISCS.addTemplate ();

  PRINTF("Done testing IsymmetricController class _KabukiToolkit");

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

  PRINTF("_::DMXControl");

  PRINT_LINE('-');
  DMXControl control_a("DMX Control A");

  PRINTF("Printing control_a.Print (_::Expr& slot)\n");
  control_a.Print();

  PRINTF("Testing copy constructor\n");
  DMXControl control_b(control_a);
  PRINTF("Printing control_b.Print (_::Expr& slot)\n");
  control_b.Print();
  PRINTF("Testing SI4 Compare  (const Control&)");

  CHECK(control_a.Compare(control_b) == 0);
  SI4 compare_value = control_a.Compare(control_b);
  PRINTF("Testing SI4 Compare  (const Control&): " + compare_value);
  CHECK(compare_value == 0);

  PRINTF("Testing valid inputs...");

  control_a = DMXControl("", 1, 1, 0, 255, 8);

  PRINTF("Printing control_a.Print (_::Expr& slot)...\n");
  control_a.Print();

  control_a.SetLabel("DMX Control B");
  CHECK(control_a.label() == "DMX Control B");
  control_a.SetInitValue(35);
  control_a.Print() CHECK(control_a.initValue() == 35);
  control_a.SetLSBValue(66);
  CHECK(control_a.lSBValue() == 66);
  control_a.SetMaxValue(245);
  CHECK(control_a.maxValue() == 245);
  control_a.SetMinValue(70);
  CHECK(control_a.GetMinValue() == 70);

  PRINTF("Retesting copy constructor");

  control_b = DMXControl(control_a);
  compare_value = control_a.Compare(control_b);
  CHECK(!compare_value);

  PRINTF("Testing invalid input...");

  control_a.SetWordSize(5);
  PRINTF("control_a.WordSize (): %i\n", control_a.WordSize());
  CHECK(control_a.GetWordSize() == 7);
  control_a.SetChannel(555);
  CHECK(control_a.GetChannel() == DMXControl::NumChannels);
  control_a.SetChannel(-5);
  CHECK(control_a.GetChannel() == 0);
  control_a.SetChannel(1);
  CHECK(control_a.GetChannel() == 1);

  PRINTF("Done testing DMXControl class SDK");

  PRINTF("_::DMXButton class SDK\n");

  DMXButton a("DMX Button A");

  PRINTF("Testing const CH1* print (_::Expr& slot)...");
  a.Print();

  PRINTF("Testing void processPress () and void processDepress ()");
  a.Press();
  CHECK(a.IsPressed());
  a.Depress();
  CHECK(!a.IsPressed());

  PRINTF("Testing void toggle ()");
  a.SetButtonAction(ButtonAction::Latching);
  a.Toggle();
  CHECK(a.GetValue() == a.GetMaxValue());
  a.Toggle();
  CHECK(a.GetValue() == a.GetMinValue());

  PRINTF("\n\n_Theater::ProjectTests completed.\n\n");

#endif
  return 0;
}
} //< namespace touch
} //< namespace toolkit
} //< namespace kabuki
