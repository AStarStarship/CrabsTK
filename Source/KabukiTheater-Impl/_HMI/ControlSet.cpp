/** Kabuki Theater
    @file    /.../Source-Impl/_HMI/ParameterSet.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <_HMI/ParameterSet.hpp>

namespace _HMI {

inline int boundRange (int const value, int const min, int const max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

ParameterSet::ParameterSet (int newNumControls)
{
    newNumControls = boundRange (newNumControls, 0, ParameterSet::MaxNumElements);

    numControls = newNumControls;
    controls = new Parameter<int>*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = nullptr;
}

ParameterSet::ParameterSet (const vector<Parameter<int>*>& newControls)
{
    int newNumControls = newControls.size ();

    if (newNumControls < MinNumElements)
    {
        // Copy what we can of the array and set the rest to nullptr.
        numControls = MinNumElements;
        controls = new Parameter<int>*[MinNumElements];

        int i;
        for (i = 0; i < newNumControls; ++i)
            controls[i] = newControls[i];

        for (; i < MinNumElements; ++i)
            controls[i] = nullptr;

        return;
    }
    numControls = newNumControls;
    controls = new Parameter<int>*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = newControls[i];
}

ParameterSet::ParameterSet (const ParameterSet& c)
{
    int newNumControls = c.numControls;
    controls = new Parameter<int>*[newNumControls];

    for (int i = 0; i < newNumControls; ++i)
        controls[i] = c.controls[i];
}

ParameterSet::~ParameterSet ()
{
    delete [] controls;
}

int ParameterSet::getCount ()
{
    return controls.size ();
}

void ParameterSet::addControl (Parameter<int>* newControl)
{
    Parameter<int>** newArray = new Parameter<int>*[numControls + 1];
    for (int i = 0; i < numControls; ++i)
        newArray[i] = controls[i];
    newArray[numControls++] = newControl;
    controls = newArray;
}

int ParameterSet::setControl (int index, Parameter<int>* c)
{
    if (index < 0) return -1;
    if (index > getCount ()) return 1;


    return 0;
}

int ParameterSet::getControl (int index, Parameter<int>* newControl)
{
    if (index < 0)
        return -1;
    if (index >= getCount ())
        return 1;

    controls[index] = newControl;
    return Success;
}

Parameter<int>* ParameterSet::removeControl (int index)
{
    if (index < 0)
        return nullptr;
    if (index >= getCount ())
        return nullptr;

    Parameter<int>* temp = controls[index];
    controls[index] = nullptr;
    return temp;
}

Parameter<int>* ParameterSet::getControl (int index)
{
    if (index < 0 || index >= getCount ())
        return nullptr;

    return controls[index];
}

int ParameterSet::getNumControls () const
{
    return numControls;
}

void ParameterSet::print () const
{
    _::printLine('~', Parameter<int>::MacroHeaderLength);
    std::cout << "Num Controls: " << getCount () << '\n';
    _::printLine('~', Parameter<int>::MacroHeaderLength);
    cout << Parameter<int>::MacroHeader << '\n';
    _::printLine ('~', Parameter<int>::MacroHeaderLength);

    printf ("numControls = %i", numControls);
    for (int i = 0; i < numControls; ++i)
    {
        auto currentControl = controls[i];

        if (currentControl == nullptr)
            printf("| nullptr  |      |      |      |     |     |       |    |        |      |\n");
        else
        {
            currentControl->printRow ();
            putchar ('\n');
        }
    }
    printf ("!!!!!!!!!!!!!!!!!!!");
    _::printLine ('~');
}

}   //< _HMI
