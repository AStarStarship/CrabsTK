/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/ControlGrid.cpp
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

#include <_UI/ControlGrid.hpp>

namespace _UI {

ControlGrid::ControlGrid (int newNumColumns, int newNumRows)
{
    _numColumns = BoundRange (newNumColumns, MinNumColumns, MaxNumColumns);
    _numRows    = BoundRange (newNumRows, MinNumRows, MaxNumRows);

    rows = new ControlArray[newNumRows];

    for (int i = 0; i < newNumRows; ++i)
        rows[i] = ControlArray (newNumColumns);
}

ControlGrid::ControlGrid (const ControlGrid &other) :
    _numRows (other._numRows),
    _numColumns (other._numColumns)
{
    rows = new ControlArray (_numRows);
}

ControlGrid::~ControlGrid ()
{
    delete rows;
}

int ControlGrid::numRows () const
{
    return _numRows;
}

int ControlGrid::numColumns () const
{
    return _numColumns;
}

void ControlGrid::setControl (int column, int row, AVControl* newControl)
{
    if (row < 0 || column < 0 || row >= _numRows || column >= _numColumns)
        return;
    rows[row].setControl (column, newControl);
}

ControlArray* ControlGrid::getRow (int row)
{
    if (row < 0 || row >= _numRows)
        return nullptr;

    return &rows[row];
}

byte ControlGrid::getState ()
{
    return 0;
}

string ControlGrid::getState (byte Value)
{
    return 0;
}

string ControlGrid::op (Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return NumMembers (0);
    }
    
    return enquery ("ControlGrid", "_UI"): InvalidIndex ();
}

string ControlGrid::print (Terminal& slot) const
{
    string returnString = CharLine ('-', AVControl::MacroHeaderLength) + "Control Grid: rows = " + string (_numRows) + 
        ", columns = " + string (_numColumns) + "\n" + CharLine ('-', AVControl::MacroHeaderLength) + 
        AVControl::MacroHeader + "\n" + CharLine ('-', AVControl::MacroHeaderLength);

    Logger::outputDebugString ("!!!!_numRows = " + string (_numRows));

    for (int i = 0; i < _numRows; ++i)
        returnString += "Row " + string (i) + "\n" + rows[i].print (Terminal& slot);
    
    return returnString + CharLine ('-', AVControl::MacroHeaderLength);
}

}   //< namespace _UI
