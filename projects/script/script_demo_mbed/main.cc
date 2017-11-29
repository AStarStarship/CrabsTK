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
