/** Kabuki Theater
    @file    /.../Source-Impl/_Math/Angle2D.inl
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


namespace _Math {

template<typename T>
Angle2D::Angle2D (float Value)
:   rads (Value)
{

}

template<typename T>
float Angle2D::getDegs () { return rads * (180.0f / M_PI); }

template<typename T>
void Angle2D::setDegs (float Value) { rads = (Value * M_PI) / 180.0f; }

template<typename T>
void Angle2D::print ()
{
    cout << header << "Angle2D: " << rads << "\n\r";
}

}   //  _Math
