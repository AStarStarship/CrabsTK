/** Kabuki Theater
    @version 0.x
    @file    /.../Source/KabukiTheater-Utils/word_size_h.hpp
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

#include <stdint.h>
#include <iostream>
#include <fstream>

/** Creates file "word_size.h" and a macro for the CPU word size.
    WORD_SIZE(s) are typically 64, 32, and 16. */
void WriteWordSizeHeaderFile () {
    std::ofstream s;
    s.open ("cpu_settings.h");
    s << "// Auto-generated file defines a macro for the CPU WORD_SIZE.\n"
        "#ifndef WORD_SIZE_H\n"
        "#define WORD_SIZE_H\n"
        "#include <stdint.h>\n"
        "#define WORD_SIZE ";
    s << sizeof (intptr_t) == 64 ? "64" : sizeof (intptr_t) == 32 ? "32" :
        sizeof (intptr_t) == 16 ? "16" : "invalid";
	s << "\n#endif  //< WORD_SIZE_H\n\r\n";
    s.close ();
}

int main () {
	WriteWordSizeHeaderFile ();
    return 0;
}
