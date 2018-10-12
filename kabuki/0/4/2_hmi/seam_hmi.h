/** Kabuki Starship
    @file /.../Source/KabukiTheater-Impl/_Theater/HMI/TESTS/ControlArrayTests.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough
   <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_Theater/HMI/ControlArray.h>
#include <_Theater/HMI/ControlMatrix.h>
#include <_Theater/HMI/MIDIControl.h>
#include <_Theater/HMI/MacroButton.h>
#include <_Theater/HMI/WidgetPage.h>

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include <stdio.h>

using namespace _;

TEST(_HMI_Tests, _Theater_HMI_ControlArrayTests_1) {
  ControlArray a();

  PRINTF("Testing const char* print (I2P::Expression& slot)...");
  a.Print();

  DMXControl c1("Control 1", 50, 33);
  MIDIControl c2("Control 2", 11, 102);

  PRINTF("testing void setControl  (Control*)");

  a.SetControl(0, &c1);
  a.SetControl(1, &c2);

  printf("Added 2 controls to a:\n");
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

  PRINTF("Attempted to add some buttons:\n");
  a.Print();

  PRINTF("Controls::ControlArray tests completed successfully. :-)");
}

TEST(_HMI_Tests, ProjectTests1) {
  printf("Controls::DMXButton class API\n");

  DMXButton a("DMX Button A");

  printf("Testing const char* print (I2P::Expression& slot)...");
  a.Print();

  printf("Testing void processPress () and void processDepress ()");
  a.Press();
  CHECK(a.isPressed());
  a.Depress();
  CHECK(!a.isPressed());

  printf("Testing void toggle ()");
  a.SetButtonAction(ButtonAction::Latching);
  a.Toggle();
  CHECK(a.GetValue() == a.GetMaxValue());
  a.Toggle();
  CHECK(a.GetValue() == a.GetMinValue());

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}

TEST(_HMI_Tests, _Theater_HMI_ControlMatrixTests_1) {
  printf("Testing Controls::ControlMatrix class.");

  printf("Testing const char* print (I2P::Expression& slot):\n");
  ControlMatrix cm(5, 2);
  cm.Print();

  printf("Testing setControl ()");

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

  printf("Done testing Controls::ControlMatrix class API! :-)");

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}

TEST(_HMI_Tests, _Theater_HMI_Widget_Tests_1) {
  printf("Running Widget tests...");

  printf("Testing Controls::Widget class API\n");
  _::printLine('~');

  Widget w("Test Widget");

  printf("Testing const char* ToString ()\n");

  w.Print();

  printf("Testing insertPage  (const char*&)\n");

  const char* pageNameString;
  char pageIndex = '1';

  for (int i = 0; i < 6; ++i) {
    pageNameString = "Page " + pageIndex;
    printf("Inserting Page %s...", pageNameString);
    CHECK(!w.insertPage(pageNameString),
          "Error: Failed to insert page " + pageNameString + "!\n");
    ++pageIndex;
  }

  printf(w.ToString());

  printf("Testing contains  (const char*&)...");

  CHECK(!w.contains("Page 1"), "Error: w does not contians \"Page 1\"");

  printf("Testing deletePage  (int)... ");

  int debugCode;

  debugCode = w.deletePage(1);

  CHECK(debugCode == 1,
        "Error: w.deletePage  (1) returrned invalid debugCode: " + debugCode);

  w.deletePage(2);
  w.deletePage(3);
  debugCode = w.deletePage(3);
  CHECK(debugCode == 1, "Error: debugCdode was not equal to 1: " + debugCode);
  w.deletePage(3);
  w.deletePage(1);
  w.deletePage(2);
  w.deletePage(2);
  w.deletePage(2);
  w.deletePage(1);
  w.deletePage(1);
  w.deletePage(1);
  w.Print();

  printf("Testing inserting duplicate page names...");

  for (int i = 0; i < 5; ++i) {
    debugCode = w.insertPage("Untitled");
    printf((i + 1) + ".) Debug code: " + debugCode);
  }
  debugCode = w.insertPage("Untitled");

  w.deletePage("Untitled 4");
  debugCode = w.insertPage("Untitled");

  printf(w.ToString());

  printf("Testing copy constructor...");
  Widget wc(w);
  wc.Print();

  printf("Done testing Widget class API");

  printf("\n\nRunning _Theater::HMI::Widget tests completed.\n\n");
}

TEST(_HMI_Tests, _Teather_HMI_WidgetPage_Tests_1) {
  printf("Testing Controls::WidgetPage");

  printf("Testing Page class\n");
  _::printLine('~');

  WidgetPage p("Test Page");

  printf("Testing const char* toString ()");
  p.Print();

  printf("Testing copy constructor...");
  WidgetPage pc(p);
  pc.Print();

  printf("Done testing Page class");

  printf("\n\nRunning _Theater::HMI::WidgetPage tests completed.\n\n");
}

TEST(_HMI_Tests, ProjectTests1) {
  printf("Testing Controls::MIDIControl\n");

  MIDIControl a("MIDI Control A");
  a.Print();

  printf("Testing setCC ()");
  a.SetCC(127);
  CHECK(a.GetCC() == 127);
  a.SetCC(128);
  CHECK(a.GetCC() == 127);
  a.SetCC(-1);
  printf("a:\n");
  a.Print();
  CHECK(a.GetCC() == 0);

  printf("Done testing Controls::MIDIControl class API. :-)\n");
}

TEST(_HMI_Tests, MIDIButtonTests1) {
  printf("Controls::MIDIButton...\n");

  MIDIButton a("MIDI Button A");

  printf("Testing const char* print (I2P::Expression& slot)");
  a.Print();

  printf("\n\n_Theater::MIDIButton tests completed.\n\n");
}

TEST(_HMI_Tests, MacroButtonTests) {
  printf("Testing Controls::MacroButton class API:\n");

  auto macroA = MacroButton("Macro Button A");

  printf("Testing const char* print (_::verifier&)...");
  printf(macroA.Print(I2P::Expression & slot));
  printf("Testing add  (Parameter<int>*)");
  macroA.add(new DMXControl("Macro 1"));
  macroA.add(new MIDIControl("Macro 2"));
  macroA.add(new DMXButton("Macro 3"));
  macroA.add(new MIDIButton("Macro 4"));
  printf("Added " + const char*(macroA.numControls()) + " test controls.\n" +
         macroA.Print(I2P::Expression & slot));
  printf("Testing void trigger ()");
  macroA.trigger();
  printf("Testing Parameter<int>* remove  (index);");

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}

TEST(_HMI_Tests, ProjectTests1) {
  printf("Controls::Label Class.\n");

  const char *invalidString = "Invalid label ~",
             *validString1 = "Valid label 1",
             *validString2 = "Valid label !@%^",
             *validString3 = "Valid label &* ()";

  Label validLabel1(validString1);
  Label validLabel2(validString1);
  Label validLabel3(validString3);
  Label invalidLabel(invalidString);

  printf("Testing valid string input.");

  printf("Testing int isValid  (const char* &)");
  printf("isValidString(validString1): %b", isValidLabel(validString1));
  CHECK(isValidLabel(validString1) == Valid);
  printf("Testing invalid string input.");
  CHECK(isValidLabel(invalidString) != Valid);
  CHECK(isValidLabel("") != 0);
  CHECK(isValidLabel("123456789012345678901") != 0);

  printf("Testing int compare  (const char* &)\n");
  invalidLabel.Print();
  printf("Testing valid comparisons...");
  CHECK(!validLabel1.Compare(validString1));
  printf("Testing invalid comparisons...");
  CHECK(validLabel1.Compare(validString2) != Comperable);
  CHECK(validLabel2.Compare(validString3) != Comperable);

  printf("Attempting to SetLabel to the InvalidString");
  // auto expectedLabel = "Test string 1";
  validLabel1.SetLabel(invalidString);
  CHECK(!validLabel1.Compare(invalidLabel));
  printf("Setting the label to validString3");
  validLabel1.SetLabel(validString3);
  CHECK(validLabel1.GetLabel() == validString3);

  printf("Done testing Controls::Label class API.");

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}

TEST(_HMI_Tests, IsymmetricControllerTests) {
  printf("Testing IsymmetricController class _KabukiToolkit\n");
  _::printLine('~');

  IsymmetricController controller();

  printf("Testing ToString ().");
  printf(controller.Print());

  // testISCS.addTemplate ();

  printf("Done testing IsymmetricController class _KabukiToolkit");

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}

TEST(_HMI_Tests, DMXControlTests) {
  printf("Controls::DMXControl");

  _::printLine('-');
  DMXControl controlA("DMX Control A");

  printf("Printing controlA.Print (I2P::Expression& slot)\n");
  controlA.Print();

  printf("Testing copy constructor\n");
  DMXControl controlB(controlA);
  printf("Printing controlB.Print (I2P::Expression& slot)\n");
  controlB.Print();
  printf("Testing int Compare  (const Control&)");

  CHECK(controlA.Compare(controlB) == 0);
  int CompareValue = controlA.Compare(controlB);
  printf("Testing int Compare  (const Control&): " + CompareValue);
  CHECK(CompareValue == 0);

  printf("Testing valid inputs...");

  controlA = DMXControl("", 1, 1, 0, 255, 8);

  printf("Printing controlA.Print (I2P::Expression& slot)...\n");
  controlA.Print();

  controlA.SetLabel("DMX Control B");
  CHECK(controlA.label() == "DMX Control B");
  controlA.SetInitValue(35);
  controlA.Print() CHECK(controlA.initValue() == 35);
  controlA.SetLSBValue(66);
  CHECK(controlA.lSBValue() == 66);
  controlA.SetMaxValue(245);
  CHECK(controlA.maxValue() == 245);
  controlA.SetMinValue(70);
  CHECK(controlA.GetMinValue() == 70);

  printf("Retesting copy constructor");

  controlB = DMXControl(controlA);
  CompareValue = controlA.Compare(controlB);
  CHECK(!CompareValue);

  printf("Testing invalid input...");

  controlA.SetWordSize(5);
  printf("controlA.wordSize (): %i\n", controlA.wordSize());
  CHECK(controlA.GetWordSize() == 7);
  controlA.SetChannel(555);
  CHECK(controlA.GetChannel() == DMXControl::NumChannels);
  controlA.SetChannel(-5);
  CHECK(controlA.GetChannel() == 0);
  controlA.SetChannel(1);
  CHECK(controlA.GetChannel() == 1);

  printf("Done testing DMXControl class API");
}

TEST(ControlArrayTests, DmxButtonTests) {
  printf("Controls::DMXButton class API\n");

  DMXButton a("DMX Button A");

  printf("Testing const char* print (I2P::Expression& slot)...");
  a.Print();

  printf("Testing void processPress () and void processDepress ()");
  a.Press();
  CHECK(a.IsPressed());
  a.Depress();
  CHECK(!a.IsPressed());

  printf("Testing void toggle ()");
  a.SetButtonAction(ButtonAction::Latching);
  a.Toggle();
  CHECK(a.GetValue() == a.GetMaxValue());
  a.Toggle();
  CHECK(a.GetValue() == a.GetMinValue());

  printf("\n\n_Theater::ProjectTests completed.\n\n");
}
