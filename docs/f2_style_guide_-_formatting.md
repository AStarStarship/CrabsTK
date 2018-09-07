F2 Foundational Framework C++ Style Guide
=========================================

## Formatting

Coding style and formatting are pretty arbitrary, but a project is much easier to follow if everyone uses the same style. Individuals may not agree with every aspect of the formatting rules, and some of the rules may take some getting used to, but it is important that all project contributors follow the style rules so that they can all read and understand everyone's code easily.

To help you format code correctly, we've created a [settings file for emacs](https://raw.githubusercontent.com/google/styleguide/gh-pages/google-c-style.el).

### Line Length

Each line of text in your code should be at most 80 characters long.

We recognize that this rule is controversial, but so much existing code already adheres to it, and we feel that consistency is important.

#### pros

Those who favor this rule argue that it is rude to force them to resize their windows and there is no need for anything longer. Some folks are used to having several code windows side-by-side, and thus don't have room to widen their windows in any case. People set up their work environment assuming a particular maximum window width, and 80 columns has been the traditional standard. Why change it?

#### cons

Proponents of change argue that a wider line can make code more readable. The 80-column limit is an hidebound throwback to 1960s mainframes; modern equipment has wide screens that can easily show longer lines.

#### decision

80 characters is the maximum.

A line may exceed 80 characters if it is

*   a comment line which is not feasible to split without harming readability, ease of cut and paste or auto-linking -- e.g. if a line contains an example command or a literal URL longer than 80 characters.
*   a raw-string literal with content that exceeds 80 characters. Except for test code, such literals should appear near the top of a file.
*   an include statement.
*   a [header guard](#The__define_Guard)
*   a using-declaration

### Non-ASCII Characters

Non-ASCII characters should be rare, and must use UTF-8 formatting.

You shouldn't hard-code user-facing text in source, even English, so use of non-ASCII characters should be rare. However, in certain cases it is appropriate to include such words in your code. For example, if your code parses data files from foreign sources, it may be appropriate to hard-code the non-ASCII string(s) used in those data files as delimiters. More commonly, unittest code (which does not need to be localized) might contain non-ASCII strings. In such cases, you should use UTF-8, since that is an encoding understood by most tools able to handle more than just ASCII.

Hex encoding is also OK, and encouraged where it enhances readability — for example, `"\xEF\xBB\xBF"`, or, even more simply, `u8"\uFEFF"`, is the Unicode zero-width no-break space character, which would be invisible if included in the source as straight UTF-8.

Use the `u8` prefix to guarantee that a string literal containing `\uXXXX` escape sequences is encoded as UTF-8. Do not use it for strings containing non-ASCII characters encoded as UTF-8, because that will produce incorrect output if the compiler does not interpret the source file as UTF-8\.

You shouldn't use the C++11 `char16_t` and `char32_t` character types, since they're for non-UTF-8 text. For similar reasons you also shouldn't use `wchar_t` (unless you're writing code that interacts with the Windows API, which uses `wchar_t` extensively).

### Spaces vs. Tabs

Use only spaces, and indent 2 spaces at a time.

We use spaces for indentation. Do not use tabs in your code. You should set your editor to emit spaces when you hit the tab key.

### Function Declarations and Definitions

Return type on the same line as function name, parameters on the same line if they fit. Wrap parameter lists which do not fit on a single line as you would wrap arguments in a [function call](#Function_Calls).

Functions look like this:

```C++
ReturnType ClassName::FunctionName(Type par_name1, Type par_name2) {
  DoSomething();
  ...
}
```

If you have too much text to fit on one line:

```C++
ReturnType ClassName::ReallyLongFunctionName(Type par_name1, Type par_name2,
                                             Type par_name3) {
  DoSomething();
  ...

}

```

or if you cannot fit even the first parameter:

```C++
ReturnType LongClassName::ReallyReallyReallyLongFunctionName(
    Type par_name1,  // 4 space indent
    Type par_name2,
    Type par_name3) {
  DoSomething();  // 2 space indent
  ...
}

```

Some points to note:

*   Choose good parameter names.
*   A parameter name may be omitted only if the parameter is not used in the function's definition.
*   If you cannot fit the return type and the function name on a single line, break between them.
*   If you break after the return type of a function declaration or definition, do not indent.
*   The open parenthesis is always on the same line as the function name.
*   There is never a space between the function name and the open parenthesis.
*   There is never a space between the parentheses and the parameters.
*   The open curly brace is always on the end of the last line of the function declaration, not the start of the next line.
*   The close curly brace is either on the last line by itself or on the same line as the open curly brace.
*   There should be a space between the close parenthesis and the open curly brace.
*   All parameters should be aligned if possible.
*   Default indentation is 2 spaces.
*   Wrapped parameters have a 4 space indent.

Unused parameters that are obvious from context may be omitted:

```C++

class Foo {

 public:

  Foo(Foo&&);

  Foo(const Foo&);

  Foo& operator=(Foo&&);

  Foo& operator=(const Foo&);

};

```

Unused parameters that might not be obvious should comment out the variable name in the function definition:

```C++

class Shape {

 public:

  virtual void Rotate(double radians) = 0;

};

class Circle : public Shape {

 public:

  void Rotate(double radians) override;

};

void Circle::Rotate(double /*radians*/) {}

```

#### badcode

```C++
// Bad - if someone wants to implement later, it's not clear what the
// variable means.

void Circle::Rotate(double) {}

```

Attributes, and macros that expand to attributes, appear at the very beginning of the function declaration or definition, before the return type:

```C++

MUST_USE_RESULT bool IsOK();

```

### Lambda Expressions

Format parameters and bodies as for any other function, and capture lists like other comma-separated lists.

For by-reference captures, do not leave a space between the ampersand (&) and the variable name.

```C++

int x = 0;

auto x_plus_n = [&x](int n) -> int { return x + n; }

```

Short lambdas may be written inline as function arguments.

```C++
std::set<int> blacklist = {7, 8, 9};
std::vector<int> digits = {3, 9, 1, 8, 4, 7, 1};
digits.erase(std::remove_if(digits.begin(), digits.end(), [&blacklist](int i) {
               return blacklist.find(i) != blacklist.end();
             }),
             digits.end());
```

### Function Calls

Either write the call all on a single line, wrap the arguments at the parenthesis, or start the arguments on a new line indented by four spaces and continue at that 4 space indent. In the absence of other considerations, use the minimum number of lines, including placing multiple arguments on each line where appropriate.

Function calls have the following format:

```C++
bool result = DoSomething(argument1, argument2, argument3);
```

If the arguments do not all fit on one line, they should be broken up onto multiple lines, with each subsequent line aligned with the first argument. Do not add spaces after the open paren or before the close paren:

```C++
bool result = DoSomething(averyveryveryverylongargument1,
                          argument2, argument3);
```

Arguments may optionally all be placed on subsequent lines with a four space indent:

```C++
if (...) {
  ...
  ...
  if (...) {
    bool result = DoSomething(
        argument1, argument2,  // 4 space indent
        argument3, argument4);
    ...
  }
```

Put multiple arguments on a single line to reduce the number of lines necessary for calling a function unless there is a specific readability problem. Some find that formatting with strictly one argument on each line is more readable and simplifies editing of the arguments. However, we prioritize for the reader over the ease of editing arguments, and most readability problems are better addressed with the following techniques.

If having multiple arguments in a single line decreases readability due to the complexity or confusing nature of the expressions that make up some arguments, try creating variables that capture those arguments in a descriptive name:

```C++
int my_heuristic = scores[x] * y + bases[x];
bool result = DoSomething(my_heuristic, x, y, z);
```

Or put the confusing argument on its own line with an explanatory comment:

```C++
bool result = DoSomething(scores[x] * y + bases[x],  // Score heuristic.
                          x, y, z);
```

If there is still a case where one argument is significantly more readable on its own line, then put it on its own line. The decision should be specific to the argument which is made more readable rather than a general policy.

Sometimes arguments form a structure that is important for readability. In those cases, feel free to format the arguments according to that structure:

```C++
// Transform the widget by a 3x3 matrix.
my_widget.Transform(x1, x2, x3,
                    y1, y2, y3,
                    z1, z2, z3);
```

### Braced Initializer List Format

Format a [braced initializer list](#Braced_Initializer_List) exactly like you would format a function call in its place.

If the braced list follows a name (e.g. a type or variable name), format as if the `{}` were the parentheses of a function call with that name. If there is no name, assume a zero-length name.

```C++
// Examples of braced init list on a single line.
return {foo, bar};
functioncall({foo, bar});
std::pair<int, int> p{foo, bar};
// When you have to wrap.
SomeFunction(
    {"assume a zero-length name before {"},
    some_other_function_parameter);
SomeType variable{
    some, other, values,
    {"assume a zero-length name before {"},
    SomeOtherType{
        "Very long string requiring the surrounding breaks.",
        some, other values},
    SomeOtherType{"Slightly shorter string",
                  some, other, values}};
SomeType variable{
    "This is too long to fit all in one line"};
MyType m = {  // Here, you could also break before {.
    superlongvariablename1,
    superlongvariablename2,
    {short, interior, list},
    {interiorwrappinglist,
     interiorwrappinglist2}};
```

### Conditionals

Prefer no spaces inside parentheses. The `if` and `else` keywords belong on separate lines.

There are two acceptable formats for a basic conditional statement. One includes spaces between the parentheses and the condition, and one does not.

The most common form is without spaces. Either is fine, but _be consistent_. If you are modifying a file, use the format that is already present. If you are writing new code, use the format that the other files in that directory or project use. If in doubt and you have no personal preference, do not add the spaces.

```C++

if (condition) {  // no spaces inside parentheses

  ...  // 2 space indent.

} else if (...) {  // The else goes on the same line as the closing brace.

  ...

} else {

  ...

}

```

If you prefer you may add spaces inside the parentheses:

```C++

if ( condition ) {  // spaces inside parentheses - rare

  ...  // 2 space indent.

} else {  // The else goes on the same line as the closing brace.

  ...

}

```

Note that in all cases you must have a space between the `if` and the open parenthesis. You must also have a space between the close parenthesis and the curly brace, if you're using one.

#### badcodeif(condition) {   // Bad - space missing after IF.

if (condition){   // Bad - space missing before {.

if(condition){    // Doubly bad.

```

```C++

if (condition) {  // Good - proper space after IF and before {.

```

Short conditional statements may be written on one line if this enhances readability. You may use this only when the line is brief and the statement does not use the `else` clause.

```C++

if (x == kFoo) return new Foo();

if (x == kBar) return new Bar();

```

This is not allowed when the if statement has an `else`:

#### badcode// Not allowed - IF statement on one line when there is an ELSE clause

if (x) DoThis();

else DoThat();

```

In general, curly braces are not required for single-line statements, but they are allowed if you like them; conditional or loop statements with complex conditions or statements may be more readable with curly braces. Some projects require that an `if` must always have an accompanying brace.

```C++

if (condition)

  DoSomething();  // 2 space indent.

if (condition) {

  DoSomething();  // 2 space indent.

}

```

However, if one part of an `if`-`else` statement uses curly braces, the other part must too:

#### badcode

```C++
// Not allowed - curly on IF but not ELSE
if (condition) {
  foo;
} else
  bar;
// Not allowed - curly on ELSE but not IF
if (condition)
  foo;
else {
  bar;
}
```

```C++
// Curly braces around both IF and ELSE required because
// one of the clauses used braces.
if (condition) {
  foo;
} else {
  bar;
}
```

### Loops and Switch Statements

Switch statements may use braces for blocks. Annotate non-trivial fall-through between cases. Braces are optional for single-statement loops. Empty loop bodies should use either empty braces or `continue`.

`case` blocks in `switch` statements can have curly braces or not, depending on your preference. If you do include curly braces they should be placed as shown below.

If not conditional on an enumerated value, switch statements should always have a `default` case (in the case of an enumerated value, the compiler will warn you if any values are not handled). If the default case should never execute, treat this as an error. For example:

```C++
switch (var) {
  case 0: {  // 2 space indent
    ...      // 4 space indent
    break;
  }
  case 1: {
    ...
    break;
  }
  default: {
    assert(false);
  }
}
```

Fall-through from one case label to another must be annotated using the `ABSL_FALLTHROUGH_INTENDED;` macro (defined in `absl/base/macros.h`). `ABSL_FALLTHROUGH_INTENDED;` should be placed at a point of execution where a fall-through to the next case label occurs. A common exception is consecutive case labels without intervening code, in which case no annotation is needed.

<div>

```C++
switch (x) {
  case 41:  // No annotation needed here.
  case 43:
    if (dont_be_picky) {
      // Use this instead of or along with annotations in comments.
      ABSL_FALLTHROUGH_INTENDED;
    } else {
      CloseButNoCigar();
      break;
    }
  case 42:
    DoSomethingSpecial();
    ABSL_FALLTHROUGH_INTENDED;
  default:
    DoSomethingGeneric();
    break;
}
```

Braces are optional for single-statement loops.

```C++
for (int i = 0; i < kSomeNumber; ++i)
  printf("I love you\n");
for (int i = 0; i < kSomeNumber; ++i) {
  printf("I take it back\n");
}
```

Empty loop bodies should use either an empty pair of braces or `continue` with no braces, rather than a single semicolon.

```C++
while (condition) {
  // Repeat test until it returns false.
}
for (int i = 0; i < kSomeNumber; ++i) {}  // Good - one newline is also OK.
while (condition) continue;  // Good - continue indicates no logic.
```

#### badcode

```C++
while (condition);  // Bad - looks like part of do/while loop.
```

### Pointer and Reference Expressions

No spaces around period or arrow. Pointer operators do not have trailing spaces.

The following are examples of correctly-formatted pointer and reference expressions:

```C++
x = *p;
p = &x;
x = r.y;
x = r->y;
```

Note that:

*   There are no spaces around the period or arrow when accessing a member.
*   Pointer operators have no space after the `*` or `&`.

When declaring a pointer variable or argument, you may place the asterisk adjacent to either the type or to the variable name:

```C++
// These are fine, space preceding.
char *c;
const string &str;
// These are fine, space following.
char* c;
const string& str;
```

You should do this consistently within a single file, so, when modifying an existing file, use the style in that file.

It is allowed (if unusual) to declare multiple variables in the same declaration, but it is disallowed if any of those have pointer or reference decorations. Such declarations are easily misread.

```C++
// Fine if helpful for readability.
int x, y;
```

#### badcode

```C++
int x, *y;  // Disallowed - no & or * in multiple declaration

char * c;  // Bad - spaces on both sides of *

const string & str;  // Bad - spaces on both sides of &

```

### Boolean Expressions

When you have a boolean expression that is longer than the [standard line length](#Line_Length), be consistent in how you break up the lines.

In this example, the logical AND operator is always at the end of the lines:

```C++
if (this_one_thing > this_other_thing &&
    a_third_thing == a_fourth_thing &&
    yet_another && last_one) {
  ...
}
```

Note that when the code wraps in this example, both of the `&&` logical AND operators are at the end of the line. This is more common in Google code, though wrapping all operators at the beginning of the line is also allowed. Feel free to insert extra parentheses judiciously because they can be very helpful in increasing readability when used appropriately. Also note that you should always use the punctuation operators, such as `&&` and `~`, rather than the word operators, such as `and` and `compl`.

### Return Values

Do not needlessly surround the `return` expression with parentheses.

Use parentheses in `return expr;` only where you would use them in `x = expr;`.

```C++
return result;                  // No parentheses in the simple case.
// Parentheses OK to make a complex expression more readable.
return (some_long_condition &&
        another_condition);
```

#### badcode

```C++
return (value);                // You wouldn't write var = (value);
return(result);                // return is not a function!
```

### Variable and Array Initialization

Your choice of `=`, `()`, or `{}`.

You may choose between `=`, `()`, and `{}`; the following are all correct:

```C++
int x = 3;
int x(3);
int x{3};
string name = "Some Name";
string name("Some Name");
string name{"Some Name"};
```

Be careful when using a braced initialization list `{...}` on a type with an `std::initializer_list` constructor. A nonempty _braced-init-list_ prefers the `std::initializer_list` constructor whenever possible. Note that empty braces `{}` are special, and will call a default constructor if available. To force the non-`std::initializer_list` constructor, use parentheses instead of braces.

```C++
std::vector<int> v(100, 1);  // A vector containing 100 items: All 1s.
std::vector<int> v{100, 1};  // A vector containing 2 items: 100 and 1.
```

Also, the brace form prevents narrowing of integral types. This can prevent some types of programming errors.

```C++
int pi(3.14);  // OK -- pi == 3.
int pi{3.14};  // Compile error: narrowing conversion.
```

### Preprocessor Directives

The hash mark that starts a preprocessor directive should always be at the beginning of the line.

Even when preprocessor directives are within the body of indented code, the directives should start at the beginning of the line.

```C++
// Good - directives at beginning of line
  if (lopsided_score) {
#if DISASTER_PENDING      // Correct -- Starts at beginning of line
    DropEverything();
# if NOTIFY               // OK but not required -- Spaces after #
    NotifyClient();
# endif
#endif
    BackToNormal();
  }
```

#### badcode

```C++
// Bad - indented directives
  if (lopsided_score) {
    #if DISASTER_PENDING   // Wrong!  The "#if" should be at beginning of line
    DropEverything();
    #endif                 // Wrong!  Do not indent #endif
    BackToNormal();
  }
```

### Class Format

Sections in `public`, `protected` and `private` order, each indented one space.

The basic format for a class definition (lacking the comments, see [Class Comments](#Class_Comments) for a discussion of what comments are needed) is:

```C++
class MyClass : public OtherClass {
 public:      // Note the 1 space indent!
  MyClass();  // Regular 2 space indent.
  explicit MyClass(int var);
  ~MyClass() {}
  void SomeFunction();
  void SomeFunctionThatDoesNothing() {}
  void set_some_var(int var) { some_var_ = var; }
  int some_var() const { return some_var_; }

 private:
  bool SomeInternalFunction();
  int some_var_;
  int some_other_var_;
};
```

Things to note:

*   Any base class name should be on the same line as the subclass name, subject to the 80-column limit.
*   The `public:`, `protected:`, and `private:` keywords should be indented one space.
*   Except for the first instance, these keywords should be preceded by a blank line. This rule is optional in small classes.
*   Do not leave a blank line after these keywords.
*   The `public` section should be first, followed by the `protected` and finally the `private` section.
*   See [Declaration Order](#Declaration_Order) for rules on ordering declarations within each of these sections.

### Constructor Initializer Lists

Constructor initializer lists can be all on one line or with subsequent lines indented four spaces.

The acceptable formats for initializer lists are:

```C++
// When everything fits on one line:
MyClass::MyClass(int var) : some_var_(var) {
  DoSomething();
}
// If the signature and initializer list are not all on one line,
// you must wrap before the colon and indent 4 spaces:
MyClass::MyClass(int var)
    : some_var_(var), some_other_var_(var + 1) {
  DoSomething();
}
// When the list spans multiple lines, put each member on its own line
// and align them:
MyClass::MyClass(int var)
    : some_var_(var),             // 4 space indent
      some_other_var_(var + 1) {  // lined up
  DoSomething();
}
// As with any other code block, the close curly can be on the same
// line as the open curly, if it fits.
MyClass::MyClass(int var)
    : some_var_(var) {}
```

### Namespace Formatting

The contents of namespaces are not indented.

[Namespaces](#Namespaces) do not add an extra level of indentation. For example, use:

```C++
namespace {
void foo() {  // Correct.  No extra indentation within namespace.
  ...
}
}  // namespace

```
Do not indent within a namespace:
#### badcode

```C++
namespace {
  // Wrong!  Indented when it should not be.
  void foo() {
    ...
  }
}  // namespace
```

When declaring nested namespaces, put each namespace on its own line.

```C++
namespace foo {
namespace bar {
}
}
```

### Horizontal Whitespace

Use of horizontal whitespace depends on location. Never put trailing whitespace at the end of a line.

#### General

```C++
void f(bool b) {  // Open braces should always have a space before them.
  ...
int i = 0;  // Semicolons usually have no space before them.
// Spaces inside braces for braced-init-list are optional.  If you use them,
// put them on both sides!
int x[] = { 0 };
int x[] = {0};
// Spaces around the colon in inheritance and initializer lists.
class Foo : public Bar {
 public:
  // For inline function implementations, put spaces between the braces
  // and the implementation itself.
  Foo(int b) : Bar(), baz_(b) {}  // No spaces inside empty braces.
  void Reset() { baz_ = 0; }  // Spaces separating braces from implementation.
  ...
};
```

Adding trailing whitespace can cause extra work for others editing the same file, when they merge, as can removing existing trailing whitespace. So: Don't introduce trailing whitespace. Remove it if you're already changing that line, or do it in a separate clean-up operation (preferably when no-one else is working on the file).

#### Loops and Conditionals

```C++
if (b) {          // Space after the keyword in conditions and loops.
} else {          // Spaces around else.
}
while (test) {}   // There is usually no space inside parentheses.
switch (i) {
int i;
for (i = 0; i < 5; ++i) {
  // Loops and conditions may have spaces inside parentheses, but this
  // is rare.  Be consistent.
}
switch (i) {
if (test) {
for (i = 0; i < 5; ++i ) {
  // For loops always have a space after the semicolon. They may have a space
  // before the semicolon, but this is rare.
}
for (; i < 5 ; ++i) {
  // ...
}
// Range-based for loops always have a space before and after the colon.
for (auto x : counts) {
  // ...
}
switch (i) {
  case 1:         // No space before colon in a switch case.
  // ...
  case 2: break;  // Use a space after a colon if there's code after it.
```

#### Operators

```C++
// Assignment operators always have spaces around them.
x = 0;
// Other binary operators usually have spaces around them, but it's
// OK to remove spaces around factors.  Parentheses should have no
// internal padding.
v = w * x + y / z;
v = w*x + y/z;
v = w * (x + z);
// No spaces separating unary operators and their arguments.
x = -5;
++x;
if (x && !y)
  ...
```

#### Templates and Casts

```C++
// No spaces inside the angle brackets (< and >), before
// <, or between >( in a cast
std::vector<string> x;
y = static_cast<char*>(x);
// Spaces between type and pointer are OK, but be consistent.
std::vector<char *> x;
```

### Vertical Whitespace

Minimize use of vertical whitespace.

This is more a principle than a rule: don't use blank lines when you don't have to. In particular, don't put more than one or two blank lines between functions, resist starting functions with a blank line, don't end functions with a blank line, and be sparing with your use of blank lines. A blank line within a block of code serves like a paragraph break in prose: visually separating two thoughts.

The basic principle is: The more code that fits on one screen, the easier it is to follow and understand the control flow of the program. Use whitespace purposefully to provide separation in that flow.

Some rules of thumb to help when blank lines may be useful:

*   Blank lines at the beginning or end of a function do not help readability.
*   Blank lines inside a chain of if-else blocks may well help readability.
*   A blank line before a comment line usually helps readability — the introduction of a new comment suggests the start of a new thought, and the blank line makes it clear that the comment goes with the following thing instead of the preceding.
