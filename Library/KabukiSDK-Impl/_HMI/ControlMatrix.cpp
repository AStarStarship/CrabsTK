/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_HMI/ControlMatrix.cpp
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

#include <_HMI/ControlMatrix.hpp>

namespace _HMI {

ControlMatrix::ControlMatrix (int newNumColumns, int newNumRows)
{
    numColumns = BoundRange (newNumColumns, MinNumColumns, MaxNumColumns);
    numRows    = BoundRange (newNumRows, MinNumRows, MaxNumRows);

    rows = new ControlArray[newNumRows];

    for (int i = 0; i < newNumRows; ++i)
        rows[i] = ControlArray (newNumColumns);
}

ControlMatrix::ControlMatrix (const ControlMatrix &other) :
    numRows (other.numRows),
    numColumns (other.numColumns)
{
    rows = new ControlArray (numRows);
}

ControlMatrix::~ControlMatrix ()
{
    delete rows;
}

int ControlMatrix::getNumRows () const
{
    return numRows;
}

int ControlMatrix::getNumColumns () const
{
    return numColumns;
}

void ControlMatrix::setControl (int column, int row, AVControl* newControl)
{
    if (row < 0 || column < 0 || row >= numRows || column >= numColumns)
        return;
    rows[row].setControl (column, newControl);
}

ControlArray* ControlMatrix::getRow (int row)
{
    if (row < 0 || row >= numRows)
        return nullptr;

    return &rows[row];
}

string ControlMatrix::print (Terminal& io) const
{
    string returnString = CharLine ('-', AVControl::MacroHeaderLength) + "Control Grid: rows = " + string (numRows) + 
        ", columns = " + string (numColumns) + "\n" + CharLine ('-', AVControl::MacroHeaderLength) + 
        AVControl::MacroHeader + "\n" + CharLine ('-', AVControl::MacroHeaderLength);

    Logger::outputDebugString ("!!!!numRows = " + string (numRows));

    for (int i = 0; i < numRows; ++i)
        returnString += "Row " + string (i) + "\n" + rows[i].print (Terminal& io);
    
    return returnString + CharLine ('-', AVControl::MacroHeaderLength);
}

}   //  _HMI
