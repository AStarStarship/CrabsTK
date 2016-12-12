/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_HMI/ControlArray.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_HMI/ControlArray.hpp>

namespace _HMI {

ControlArray::ControlArray (int newNumControls)
{
    newNumControls = BoundRange (newNumControls, 0, ControlArray::MaxNumElements);

    numControls = newNumControls;
    controls = new AVControl*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = nullptr;
}

ControlArray::ControlArray (const Array<AVControl*>& newControls)
{
    int newNumControls = newControls.size ();

    if (newNumControls < MinNumElements)
    {
        // Copy what we can of the array and set the rest to nullptr.
        numControls = MinNumElements;
        controls = new AVControl*[MinNumElements];

        int i;
        for (i = 0; i < newNumControls; ++i)
            controls[i] = newControls[i];

        for (; i < MinNumElements; ++i)
            controls[i] = nullptr;

        return;
    }
    numControls = newNumControls;
    controls = new AVControl*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = newControls[i];
}

ControlArray::ControlArray (const ControlArray& other)
{
    int newNumControls = other.numControls;
    controls = new AVControl*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = other.controls[i];
}

ControlArray::~ControlArray ()
{
    delete [] controls;
}

void ControlArray::addControl (AVControl* newControl)
{
    AVControl** newArray = new AVControl*[numControls + 1];
    for (int i = 0; i < numControls; ++i)
        newArray[i] = controls[i];
    newArray[numControls++] = newControl;
    controls = newArray;
}

int ControlArray::getControl (int index, AVControl* newControl)
{
    if (index < 0)
        return -1;
    if (index >= numControls)
        return 1;

    controls[index] = newControl;
    return Success;
}

AVControl* ControlArray::removeControl (int index)
{
    if (index < 0)
        return nullptr;
    if (index >= numControls ())
        return nullptr;

    AVControl* temp = controls[index];
    controls[index] = nullptr;
    return temp;
}

AVControl* ControlArray::getControl (int index)
{
    if (index < 0 || index >= numControls)
        return nullptr;

    return controls[index];
}

int ControlArray::getNumControls () const
{
    return numControls;
}

string ControlArray::print (Terminal& slot) const
{
    string returnString = CharLine ('~', AVControl::MacroHeaderLength) + "Num Controls: " + string (numControls) + 
        "\n" + CharLine ('~', AVControl::MacroHeaderLength) + AVControl::MacroHeader + "\n" + 
        CharLine ('~', AVControl::MacroHeaderLength);

    Logger::outputDebugString ("numControls = " + string (numControls));
    for (int i = 0; i < numControls; ++i)
    {
        auto currentControl = controls[i];

        if (currentControl == nullptr)
            returnString += "| nullptr  |      |      |      |     |     |       |    |        |      |\n";
        else
            returnString += currentControl->toStringRow () + "\n";
    }
    Logger::outputDebugString ("!!!!!!!!!!!!!!!!!!!");
    return returnString += CharLine ('~', AVControl::MacroHeaderLength);
}

}   //  _HMI
