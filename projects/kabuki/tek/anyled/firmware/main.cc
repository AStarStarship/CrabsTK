/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/tek/anyled/firmware/pch.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "mbed.h"
#include "../../../source/kabuki/script/global.h"
#include "../../../source/kabuki/script/expression.h"
using namespace _;

DigitalOut myled(LED1);

class DemoRoom : public Room {
public:

	DemoRoom ():
	    Room ("Chinese_Room") {

	}
};

int main() {

    DemoRoom room;
    Expression expr;
    void* args[1];
    uint32_t test_var;
    ExprArgs (&expr, Params<1, UI4> (), Args (args, &test_var));
    Bin bin;
    Bout bout;
    BoutPortal bio (&bin, &bout);
    ExpressionScan (&expr, &bio);
    ExprResult (&expr, Params<1, UI4> (), Args (args, &test_var));

    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}
