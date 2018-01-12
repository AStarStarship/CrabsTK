/* Script uses C++ comments and Doxygen notation.
   Script is designed for natural language processing so delimiters are the 
   non-alpha-numeric ASCII chars and any symbol key may be user-defined as
   either a Operation (function call), Operand (push object onto stack), or
   an Escape Sequence Macro.

   There are three functions in this example: Foo, Bar, an FooBar. They take 
   the respective input and output parameters:
        +--------------- 4 parameters
        |   +----------- A string of max length 31.
        |   |       +--- A signed integer with 1-byte (8-bit) width.
        |   |       |
        |   |       |               +-- NIL is 0 so there are 0 parameters.
        |   |       |               |
        v   v       v               v
   Foo <4, STX:32, SI1, SI2, SI4>:<NIL>
   Bar <1, STX:32>:<3, SI1, SI2, SI4>
   FooBar <DIC2, UI1>:<NIL>
            ^     ^
            |     |
            |     +----- An 8-bit unsigned integer.
            +----------- A 16-bit dictionary.
*/

Parent.Child1.Foo ("Hello world!", 1, 2, 3);
Parent Child1 Foo "Hello world!", 1, 2, 3;//< () and ',' are optional
Parent Child1 Foo "Hello world!" 1 2 3    //< There is no pop instruction here
              Foo "Hello "                //< so this is a string of Operations.
                  "world!" 1 2
                  3;                      //< The Last 2 Foo calls execute the 
                                          //< ";" function is read.
Parent {
    Child1 {
        Foo ("Hello world!", 1, 2, 3)
        Foo ("Hello world!", 1, 2, 3)    //< String function calls together!
        Bar ("Lets go!")
    }
    Child2 {
        Foo ("Hello world!" 1, 2, 3)
    }
} //< Code won't execute until you close the curly brackets or type ';'
Parent1 {
    Child1 {
        Foo ("Hello world!", 1, 2, 3)
/* Below is an example if an Escape Sequence Macro that may be used in function 
   to pass parameters and chain Operations. Script's multiple return parameters 
   allows b-sequences to be used to pass some but not all of the parameters 
   into another Operation. */
Foo ("Bar goes the programmer." .Parent1.Child1.Bar ("Lets go!"))

//                 +-------------------- '.' pushes the root Operand on stack.
//                 |                     It helps speed up key resolution but is
//                 |                     not required because any key not in
//                 |                     the current scope will get checked for 
//                 |                     in the root scope if it's not in the 
//                 |                     current scope.
//                 v                      
let esc-sequence = .Parent.Child1.Bar ("This is an Escape sequence!");
.Parent1.Child1.Foo ("This is an Escape Sequence", esc-sequence);
                                      
/* Dictionary Example.
   The dictionary format is actually a string of Operations calls with 
   the string key "+" and '-'.

 +-------------------------------------- This is a function with key "let" that 
 |                                       takes a string Token (TKN). A TKN is
 |                                       a ASCII or UTF-8 string without
 |                                       any Script Whitespace or symbols 
 |                                       (@see Script Spec).
 |                 +-------------------- This is a NoOp Operation that is only
 |                 |                     here to visual separate .
 |                 | +------------------ Operand "$" only evaluates the 
 |                 | |                   b-sequence once like static const.
 |                 | | +---------------- This is a function called "DIC2" that 
 |                 | | |                 creates a dictionary that is size 1024 
 |                 | | |                 bytes with a maximum of 3 entries and 
 |                 | | |                 pushes it onto the stack.
 |                 | | |
 v                 V v v                 */
let new_dictionary = $DIC2 (1024, 3) {
/*< 
    +----------------------------------- Operation with key "+DIC2" identical 
    |                                    to "$DIC2" except it will be evaluated 
    |                                    each time it gets called, which is 
    |                                    once in this context.
    |    +------------------------------ This is an Operand with key "DIC2".
    |    |     +------------------------ Same "DIC2" Operand as above.
    |    |     |    +------------------- This reserves 3 dictionary members.
    |    |     |    |
    v    v     v    v */
    +DIC2 ("item1", 1024, 3) {        // This is a nested dictionary.
        //   +-------------------------- This is a operation with key "STX"
        //   |   +---------------------- Dictionary key.
        //   |   |     +---------------- Max length 12.
        //   |   |     |       +-------- STX value.
        //   |   |     |       |
        //   v   v     v       v
        +STX ("item1" 12 "Hello world!") }
        -Key "item1"             //  Operation "-Key" removes "item1".
            
        //  +--------------------------- This is a function with key "UI2".
        //  |        +------------------ This is a 16-bit unsigned int.
        //  |        |
        //  v        v
        +UI2 "item1" 1
        +UI2 ("item2", 2)
        +UI2 "item3" 1
        -Index 2                     //< Removes index 2:"item3".
        +SI2 "item3" -1
        +UI2 "item4" 1
        /*< We can't add item4 because we set the max elements to 3.
            This won't generate an error but may generate an error 
            message. */
        -All                         //< Removes everything we just added.
    }
}

FooBar (new_dictionary, 1);
