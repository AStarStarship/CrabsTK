/** Kabuki Theater
    @file    /.../Source-Impl/_HMI/ControlMatrix.cpp
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

#include <_HMI/ControlMatrix.h>

namespace _HMI {

ControlMatrix::ControlMatrix (int newNumColumns, int newNumRows)
{
    numColumns = _::boundRange (newNumColumns, MinNumColumns, MaxNumColumns);
    numRows    = _::boundRange (newNumRows, MinNumRows, MaxNumRows);

    rows = new ParameterSet[newNumRows];

    for (int i = 0; i < newNumRows; ++i)
        rows[i] = ParameterSet (newNumColumns);
}

ControlMatrix::ControlMatrix (const ControlMatrix &o) :
    numRows (o.numRows),
    numColumns (o.numColumns)
{
    rows = new ParameterSet (numRows);
}

ControlMatrix::~ControlMatrix ()
{
    if (rows == nullptr) return;
    delete [] rows;
}

int ControlMatrix::getNumRows () const
{
    return numRows;
}

int ControlMatrix::getNumColumns () const
{
    return numColumns;
}

void ControlMatrix::setControl (int column, int row, Parameter<int>* newControl)
{
    if (row < 0 || column < 0 || row >= numRows || column >= numColumns)
        return;
    rows[row].setControl (column, newControl);
}

ParameterSet* ControlMatrix::getRow (int row)
{
    if (row < 0 || row >= numRows)
        return nullptr;

    return &rows[row];
}

void ControlMatrix::print() const
{
    _::printLine ('-', Parameter<int>::MacroHeaderLength);
    printf ("Control Grid: rows = % , columns = %i\n", numRows, numColumns);
    _::printLine ('-', Parameter<int>::MacroHeaderLength);
    printMacroHeader ();
    _::printNL ();
    printLine ('-', Parameter<int>::MacroHeaderLength);

    printf ("!!!!numRows = %i\n", numRows);

    for (int i = 0; i < numRows; ++i)
    {
        printf ("Row %i:\n", i);
        rows[i].print ();
    }
    
    printLine ('-', Parameter<int>::MacroHeaderLength);
}

}   //< _HMI
