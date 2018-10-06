F2 Foundational Framework C++ Style Guide
=========================================

## Google-Specific Magic

There are various tricks and utilities that we use to make C++ code more robust, and various ways we use C++ that may differ from what you see elsewhere.

### Ownership and Smart Pointers

Prefer to have single, fixed owners for dynamically allocated objects. Prefer to transfer ownership with smart pointers.

#### definition

"Ownership" is a bookkeeping technique for managing dynamically allocated memory (and other resources). The owner of a dynamically allocated object is an object or function that is responsible for ensuring that it is deleted when no longer needed. Ownership can sometimes be shared, in which case the last owner is typically responsible for deleting it. Even when ownership is not shared, it can be transferred from one piece of code to another.

"Smart" pointers are classes that act like pointers, e.g. by overloading the `*` and `->` operators. Some smart pointer types can be used to automate ownership bookkeeping, to ensure these responsibilities are met. [`std::unique_ptr`](http://en.cppreference.com/w/cpp/memory/unique_ptr) is a smart pointer type introduced in C++11, which expresses exclusive ownership of a dynamically allocated object; the object is deleted when the `std::unique_ptr` goes out of scope. It cannot be copied, but can be _moved_ to represent ownership transfer. [`std::shared_ptr`](http://en.cppreference.com/w/cpp/memory/shared_ptr) is a smart pointer type that expresses shared ownership of a dynamically allocated object. `std::shared_ptr`s can be copied; ownership of the object is shared among all copies, and the object is deleted when the last `std::shared_ptr` is destroyed.

#### pros

*   It's virtually impossible to manage dynamically allocated memory without some sort of ownership logic.

*   Transferring ownership of an object can be cheaper than copying it (if copying it is even possible).

*   Transferring ownership can be simpler than 'borrowing' a pointer or reference, because it reduces the need to coordinate the lifetime of the object between the two users.

*   Smart pointers can improve readability by making ownership logic explicit, self-documenting, and unambiguous.

*   Smart pointers can eliminate manual ownership bookkeeping, simplifying the code and ruling out large classes of errors.

*   For const objects, shared ownership can be a simple and efficient alternative to deep copying.

#### cons

*   Ownership must be represented and transferred via pointers (whether smart or plain). Pointer semantics are more complicated than value semantics, especially in APIs: you have to worry not just about ownership, but also aliasing, lifetime, and mutability, among other issues.

*   The performance costs of value semantics are often overestimated, so the performance benefits of ownership transfer might not justify the readability and complexity costs.

*   APIs that transfer ownership force their clients into a single memory management model.

*   Code using smart pointers is less explicit about where the resource releases take place.

*   `std::unique_ptr` expresses ownership transfer using C++11's move semantics, which are relatively new and may confuse some programmers.

*   Shared ownership can be a tempting alternative to careful ownership design, obfuscating the design of a system.

*   Shared ownership requires explicit bookkeeping at run-time, which can be costly.

*   In some cases (e.g. cyclic references), objects with shared ownership may never be deleted.

*   Smart pointers are not perfect substitutes for plain pointers.

#### decision

If dynamic allocation is necessary, prefer to keep ownership with the code that allocated it. If other code needs access to the object, consider passing it a copy, or passing a pointer or reference without transferring ownership. Prefer to use `std::unique_ptr` to make ownership transfer explicit. For example:

```C++

std::unique_ptr<Foo> FooFactory();

void FooConsumer(std::unique_ptr<Foo> ptr);

```

Do not design your code to use shared ownership without a very good reason. One such reason is to avoid expensive copy operations, but you should only do this if the performance benefits are significant, and the underlying object is immutable (i.e. `std::shared_ptr<const Foo>`). If you do use shared ownership, prefer to use `std::shared_ptr`.

Never use `std::auto_ptr`. Instead, use `std::unique_ptr`.

### cpplint

Use `cpplint.py` to detect style errors.

`cpplint.py` is a tool that reads a source file and identifies many style errors. It is not perfect, and has both false positives and false negatives, but it is still a valuable tool. False positives can be ignored by putting `// NOLINT` at the end of the line or `// NOLINTNEXTLINE` in the previous line.

Some projects have instructions on how to run `cpplint.py` from their project tools. If the project you are contributing to does not, you can download [`cpplint.py`](https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py) separately.

## Other C++ Features

### Rvalue References

Use rvalue references to:

*   Define move constructors and move assignment operators.

*   Define [overload sets](#Function_Overloading) with const& and && variants if you have evidence that this provides meaningfully better performance than passing by value, or if you're writing low-overhead generic code that needs to support arbitrary types. Beware combinatorial overload sets, that is, seldom overload more than one parameter.

*   Support 'perfect forwarding' in generic code.

#### definition

Rvalue references are a type of reference that can only bind to temporary objects. The syntax is similar to traditional reference syntax. For example, `void f(string&& s);` declares a function whose argument is an rvalue reference to a string.

When the token '&&' is applied to an unqualified template argument in a function parameter, special template argument deduction rules apply. Such a reference is called forwarding reference.

#### pros

*   Defining a move constructor (a constructor taking an rvalue reference to the class type) makes it possible to move a value instead of copying it. If `v1` is a `std::vector<string>`, for example, then `auto v2(std::move(v1))` will probably just result in some simple pointer manipulation instead of copying a large amount of data. In many cases this can result in a major performance improvement.

*   Rvalue references make it possible to implement types that are movable but not copyable, which can be useful for types that have no sensible definition of copying but where you might still want to pass them as function arguments, put them in containers, etc.

*   `std::move` is necessary to make effective use of some standard-library types, such as `std::unique_ptr`.

*   [Forwarding references](#Forwarding_references) which use the rvalue reference token, make it possible to write a generic function wrapper that forwards its arguments to another function, and works whether or not its arguments are temporary objects and/or const. This is called 'perfect forwarding'.

#### cons

*   Rvalue references are not yet widely understood. Rules like automatic synthesis of move constructors and reference collapsing (the latter refers to the special rules that apply to a T&& parameter in a function template) are somewhat obscure.

*   Rvalue references are often misused. Using rvalue references is counter-intuitive in signatures where the argument is expected to have a valid specified state after the function call, or where no move operation is performed.

#### decision

You may use rvalue references to define move constructors and move assignment operators (as described in [Copyable and Movable Types](#Copyable_Movable_Types)). See the [C++ Primer](primer#copying_moving) for more information about move semantics and `std::move`.

You may use rvalue references to define pairs of overloads, one taking Foo&& and the other taking const Foo&. Usually the preferred solution is just to pass by value, but an overloaded pair of functions sometimes yields better performance and is sometimes necessary in generic code that needs to support a wide variety of types. As always: if you're writing more complicated code for the sake of performance, make sure you have evidence that it actually helps.

You may use forwarding references in conjunction with `[std::forward](http://en.cppreference.com/w/cpp/utility/forward)`, to support perfect forwarding.

### Friends

We allow use of `friend` classes and functions, within reason.

Friends should usually be defined in the same file so that the reader does not have to look in another file to find uses of the private members of a class. A common use of `friend` is to have a `FooBuilder` class be a friend of `Foo` so that it can construct the inner state of `Foo` correctly, without exposing this state to the world. In some cases it may be useful to make a unittest class a friend of the class it tests.

Friends extend, but do not break, the encapsulation boundary of a class. In some cases this is better than making a member public when you want to give only one other class access to it. However, most classes should interact with other classes solely through their public members.

### Exceptions

We do not use C++ exceptions.

#### pros

*   Exceptions allow higher levels of an application to decide how to handle "can't happen" failures in deeply nested functions, without the obscuring and error-prone bookkeeping of error codes.

*   Exceptions are used by most other modern languages. Using them in C++ would make it more consistent with Python, Java, and the C++ that others are familiar with.

*   Some third-party C++ libraries use exceptions, and turning them off internally makes it harder to integrate with those libraries.

*   Exceptions are the only way for a constructor to fail. We can simulate this with a factory function or an `Init()` method, but these require heap allocation or a new "invalid" state, respectively.

*   Exceptions are really handy in testing frameworks.

#### cons

*   When you add a `throw` statement to an existing function, you must examine all of its transitive callers. Either they must make at least the basic exception safety guarantee, or they must never catch the exception and be happy with the program terminating as a result. For instance, if `f()` calls `g()` calls `h()`, and `h` throws an exception that `f` catches, `g` has to be careful or it may not clean up properly.

*   More generally, exceptions make the control flow of programs difficult to evaluate by looking at code: functions may return in places you don't expect. This causes maintainability and debugging difficulties. You can minimize this cost via some rules on how and where exceptions can be used, but at the cost of more that a developer needs to know and understand.

*   Exception safety requires both RAII and different coding practices. Lots of supporting machinery is needed to make writing correct exception-safe code easy. Further, to avoid requiring readers to understand the entire call graph, exception-safe code must isolate logic that writes to persistent state into a "commit" phase. This will have both benefits and costs (perhaps where you're forced to obfuscate code to isolate the commit). Allowing exceptions would force us to always pay those costs even when they're not worth it.

*   Turning on exceptions adds data to each binary produced, increasing compile time (probably slightly) and possibly increasing address space pressure.

*   The availability of exceptions may encourage developers to throw them when they are not appropriate or recover from them when it's not safe to do so. For example, invalid user input should not cause exceptions to be thrown. We would need to make the style guide even longer to document these restrictions!

#### decision

On their face, the benefits of using exceptions outweigh the costs, especially in new projects. However, for existing code, the introduction of exceptions has implications on all dependent code. If exceptions can be propagated beyond a new project, it also becomes problematic to integrate the new project into existing exception-free code. Because most existing C++ code at Google is not prepared to deal with exceptions, it is comparatively difficult to adopt new code that generates exceptions.

Given that Google's existing code is not exception-tolerant, the costs of using exceptions are somewhat greater than the costs in a new project. The conversion process would be slow and error-prone. We don't believe that the available alternatives to exceptions, such as error codes and assertions, introduce a significant burden.

Our advice against using exceptions is not predicated on philosophical or moral grounds, but practical ones. Because we'd like to use our open-source projects at Google and it's difficult to do so if those projects use exceptions, we need to advise against exceptions in Google open-source projects as well. Things would probably be different if we had to do it all over again from scratch.

This prohibition also applies to the exception handling related features added in C++11, such as `std::exception_ptr` and `std::nested_exception`.

There is an [exception](#Windows_Code) to this rule (no pun intended) for Windows code.

### `noexcept`

Specify `noexcept` when it is useful and correct.

#### definition

The `noexcept` specifier is used to specify whether a function will throw exceptions or not. If an exception escapes from a function marked `noexcept`, the program crashes via `std::terminate`.

The `noexcept` operator performs a compile-time check that returns true if an expression is declared to not throw any exceptions.

#### pros

*   Specifying move constructors as `noexcept` improves performance in some cases, e.g. `std::vector<T>::resize()` moves rather than copies the objects if T's move constructor is `noexcept`.

*   Specifying `noexcept` on a function can trigger compiler optimizations in environments where exceptions are enabled, e.g. compiler does not have to generate extra code for stack-unwinding, if it knows that no exceptions can be thrown due to a `noexcept` specifier.

#### cons

*   In projects following this guide that have exceptions disabled it is hard to ensure that `noexcept` specifiers are correct, and hard to define what correctness even means.

*   It's hard, if not impossible, to undo `noexcept` because it eliminates a guarantee that callers may be relying on, in ways that are hard to detect.

#### decision

You may use `noexcept` when it is useful for performance if it accurately reflects the intended semantics of your function, i.e. that if an exception is somehow thrown from within the function body then it represents a fatal error. You can assume that `noexcept` on move constructors has a meaningful performance benefit. If you think there is significant performance benefit from specifying `noexcept` on some other function, please discuss it with your project leads.

Prefer unconditional `noexcept` if exceptions are completely disabled (i.e. most Google C++ environments). Otherwise, use conditional `noexcept` specifiers with simple conditions, in ways that evaluate false only in the few cases where the function could potentially throw. The tests might include type traits check on whether the involved operation might throw (e.g. `std::is_nothrow_move_constructible` for move-constructing objects), or on whether allocation can throw (e.g. `absl::default_allocator_is_nothrow` for standard default allocation). Note in many cases the only possible cause for an exception is allocation failure (we believe move constructors should not throw except due to allocation failure), and there are many applications where it’s appropriate to treat memory exhaustion as a fatal error rather than an exceptional condition that your program should attempt to recover from. Even for other potential failures you should prioritize interface simplicity over supporting all possible exception throwing scenarios: instead of writing a complicated `noexcept` clause that depends on whether a hash function can throw, for example, simply document that your component doesn’t support hash functions throwing and make it unconditionally `noexcept`.

### Run-Time Type Information (RTTI)

Avoid using Run Time Type Information (RTTI).

#### definition

RTTI allows a programmer to query the C++ class of an object at run time. This is done by use of `typeid` or `dynamic_cast`.

#### cons

Querying the type of an object at run-time frequently means a design problem. Needing to know the type of an object at runtime is often an indication that the design of your class hierarchy is flawed.

Undisciplined use of RTTI makes code hard to maintain. It can lead to type-based decision trees or switch statements scattered throughout the code, all of which must be examined when making further changes.

#### pros

The standard alternatives to RTTI (described below) require modification or redesign of the class hierarchy in question. Sometimes such modifications are infeasible or undesirable, particularly in widely-used or mature code.

RTTI can be useful in some unit tests. For example, it is useful in tests of factory classes where the test has to verify that a newly created object has the expected dynamic type. It is also useful in managing the relationship between objects and their mocks.

RTTI is useful when considering multiple abstract objects. Consider

```C++

bool Base::Equal(Base* other) = 0;

bool Derived::Equal(Base* other) {

  Derived* that = dynamic_cast<Derived*>(other);

  if (that == nullptr)

    return false;

  ...

}

```

#### decision

RTTI has legitimate uses but is prone to abuse, so you must be careful when using it. You may use it freely in unittests, but avoid it when possible in other code. In particular, think twice before using RTTI in new code. If you find yourself needing to write code that behaves differently based on the class of an object, consider one of the following alternatives to querying the type:

*   Virtual methods are the preferred way of executing different code paths depending on a specific subclass type. This puts the work within the object itself.

*   If the work belongs outside the object and instead in some processing code, consider a double-dispatch solution, such as the Visitor design pattern. This allows a facility outside the object itself to determine the type of class using the built-in type system.

When the logic of a program guarantees that a given instance of a base class is in fact an instance of a particular derived class, then a `dynamic_cast` may be used freely on the object. Usually one can use a `static_cast` as an alternative in such situations.

Decision trees based on type are a strong indication that your code is on the wrong track.

#### badcodeif (typeid(*data) == typeid(D1)) {

  ...

} else if (typeid(*data) == typeid(D2)) {

  ...

} else if (typeid(*data) == typeid(D3)) {

...

```

Code such as this usually breaks when additional subclasses are added to the class hierarchy. Moreover, when properties of a subclass change, it is difficult to find and modify all the affected code segments.

Do not hand-implement an RTTI-like workaround. The arguments against RTTI apply just as much to workarounds like class hierarchies with type tags. Moreover, workarounds disguise your true intent.

### Casting

Use C++-style casts like `static_cast<float>(double_value)`, or brace initialization for conversion of arithmetic types like `int64 y = int64{1} << 42`. Do not use cast formats like `int y = (int)x` or `int y = int(x)` (but the latter is okay when invoking a constructor of a class type).

#### definition

C++ introduced a different cast system from C that distinguishes the types of cast operations.

#### pros

The problem with C casts is the ambiguity of the operation; sometimes you are doing a _conversion_ (e.g., `(int)3.5`) and sometimes you are doing a _cast_ (e.g., `(int)"hello"`). Brace initialization and C++ casts can often help avoid this ambiguity. Additionally, C++ casts are more visible when searching for them.

#### cons

The C++-style cast syntax is verbose and cumbersome.

#### decision

Do not use C-style casts. Instead, use these C++-style casts when explicit type conversion is necessary.

*   Use brace initialization to convert arithmetic types (e.g. `int64{x}`). This is the safest approach because code will not compile if conversion can result in information loss. The syntax is also concise.

*   Use `static_cast` as the equivalent of a C-style cast that does value conversion, when you need to explicitly up-cast a pointer from a class to its superclass, or when you need to explicitly cast a pointer from a superclass to a subclass. In this last case, you must be sure your object is actually an instance of the subclass.

*   Use `const_cast` to remove the `const` qualifier (see [const](#Use_of_const)).

*   Use `reinterpret_cast` to do unsafe conversions of pointer types to and from integer and other pointer types. Use this only if you know what you are doing and you understand the aliasing issues.

See the [RTTI section](#Run-Time_Type_Information__RTTI_) for guidance on the use of `dynamic_cast`.

### Streams

Use streams where appropriate, and stick to "simple" usages. Overload `<<` for streaming only for types representing values, and write only the user-visible value, not any implementation details.

#### definition

Streams are the standard I/O abstraction in C++, as exemplified by the standard header `<iostream>`. They are widely used in Google code, but only for debug logging and test diagnostics.

#### pros

The `<<` and `>>` stream operators provide an API for formatted I/O that is easily learned, portable, reusable, and extensible. `printf`, by contrast, doesn't even support `string`, to say nothing of user-defined types, and is very difficult to use portably. `printf` also obliges you to choose among the numerous slightly different versions of that function, and navigate the dozens of conversion specifiers.

Streams provide first-class support for console I/O via `std::cin`, `std::cout`, `std::cerr`, and `std::clog`. The C APIs do as well, but are hampered by the need to manually buffer the input.

#### cons

*   Stream formatting can be configured by mutating the state of the stream. Such mutations are persistent, so the behavior of your code can be affected by the entire previous history of the stream, unless you go out of your way to restore it to a known state every time other code might have touched it. User code can not only modify the built-in state, it can add new state variables and behaviors through a registration system.

*   It is difficult to precisely control stream output, due to the above issues, the way code and data are mixed in streaming code, and the use of operator overloading (which may select a different overload than you expect).

*   The practice of building up output through chains of `<<` operators interferes with internationalization, because it bakes word order into the code, and streams' support for localization is [flawed](http://www.boost.org/doc/libs/1_48_0/libs/locale/doc/html/rationale.html#rationale_why).

*   The streams API is subtle and complex, so programmers must develop experience with it in order to use it effectively.

*   Resolving the many overloads of `<<` is extremely costly for the compiler. When used pervasively in a large code base, it can consume as much as 20% of the parsing and semantic analysis time.

#### decision

Use streams only when they are the best tool for the job. This is typically the case when the I/O is ad-hoc, local, human-readable, and targeted at other developers rather than end-users. Be consistent with the code around you, and with the codebase as a whole; if there's an established tool for your problem, use that tool instead. In particular, logging libraries are usually a better choice than `std::cerr` or `std::clog` for diagnostic output, and the libraries in `absl/strings` or the equivalent are usually a better choice than `std::stringstream`.

Avoid using streams for I/O that faces external users or handles untrusted data. Instead, find and use the appropriate templating libraries to handle issues like internationalization, localization, and security hardening.

If you do use streams, avoid the stateful parts of the streams API (other than error state), such as `imbue()`, `xalloc()`, and `register_callback()`. Use explicit formatting functions (see e.g. `absl/strings`) rather than stream manipulators or formatting flags to control formatting details such as number base, precision, or padding.

Overload `<<` as a streaming operator for your type only if your type represents a value, and `<<` writes out a human-readable string representation of that value. Avoid exposing implementation details in the output of `<<`; if you need to print object internals for debugging, use named functions instead (a method named `DebugString()` is the most common convention).

### Preincrement and Predecrement

Use prefix form (`++i`) of the increment and decrement operators with iterators and other template objects.

#### definition

When a variable is incremented (`++i` or `i++`) or decremented (`--i` or `i--`) and the value of the expression is not used, one must decide whether to preincrement (decrement) or postincrement (decrement).

#### pros

When the return value is ignored, the "pre" form (`++i`) is never less efficient than the "post" form (`i++`), and is often more efficient. This is because post-increment (or decrement) requires a copy of `i` to be made, which is the value of the expression. If `i` is an iterator or other non-scalar type, copying `i` could be expensive. Since the two types of increment behave the same when the value is ignored, why not just always pre-increment?

#### cons

The tradition developed, in C, of using post-increment when the expression value is not used, especially in `for` loops. Some find post-increment easier to read, since the "subject" (`i`) precedes the "verb" (`++`), just like in English.

#### decision

For simple scalar (non-object) values there is no reason to prefer one form and we allow either. For iterators and other template types, use pre-increment.

### Use of const

Use `const` whenever it makes sense. With C++11, `constexpr` is a better choice for some uses of const.

#### definition

Declared variables and parameters can be preceded by the keyword `const` to indicate the variables are not changed (e.g., `const int foo`). Class functions can have the `const` qualifier to indicate the function does not change the state of the class member variables (e.g., `class Foo { int Bar(char c) const; };`).

#### pros

Easier for people to understand how variables are being used. Allows the compiler to do better type checking, and, conceivably, generate better code. Helps people convince themselves of program correctness because they know the functions they call are limited in how they can modify your variables. Helps people know what functions are safe to use without locks in multi-threaded programs.

#### cons

`const` is viral: if you pass a `const` variable to a function, that function must have `const` in its prototype (or the variable will need a `const_cast`). This can be a particular problem when calling library functions.

#### decision

`const` variables, data members, methods and arguments add a level of compile-time type checking; it is better to detect errors as soon as possible. Therefore we strongly recommend that you use `const` whenever it makes sense to do so:

*   If a function guarantees that it will not modify an argument passed by reference or by pointer, the corresponding function parameter should be a reference-to-const (`const T&`) or pointer-to-const (`const T*`), respectively.

*   Declare methods to be `const` whenever possible. Accessors should almost always be `const`. Other methods should be const if they do not modify any data members, do not call any non-`const` methods, and do not return a non-`const` pointer or non-`const` reference to a data member.

*   Consider making data members `const` whenever they do not need to be modified after construction.

The `mutable` keyword is allowed but is unsafe when used with threads, so thread safety should be carefully considered first.

#### stylepoint_subsection

#### Where to put the const

Some people favor the form `int const *foo` to `const int* foo`. They argue that this is more readable because it's more consistent: it keeps the rule that `const` always follows the object it's describing. However, this consistency argument doesn't apply in codebases with few deeply-nested pointer expressions since most `const` expressions have only one `const`, and it applies to the underlying value. In such cases, there's no consistency to maintain. Putting the `const` first is arguably more readable, since it follows English in putting the "adjective" (`const`) before the "noun" (`int`).

That said, while we encourage putting `const` first, we do not require it. But be consistent with the code around you!

### Use of constexpr

In C++11, use `constexpr` to define true constants or to ensure constant initialization.

#### definition

Some variables can be declared `constexpr` to indicate the variables are true constants, i.e. fixed at compilation/link time. Some functions and constructors can be declared `constexpr` which enables them to be used in defining a `constexpr` variable.

#### pros

Use of `constexpr` enables definition of constants with floating-point expressions rather than just literals; definition of constants of user-defined types; and definition of constants with function calls.

#### cons

Prematurely marking something as constexpr may cause migration problems if later on it has to be downgraded. Current restrictions on what is allowed in constexpr functions and constructors may invite obscure workarounds in these definitions.

#### decision

`constexpr` definitions enable a more robust specification of the constant parts of an interface. Use `constexpr` to specify true constants and the functions that support their definitions. Avoid complexifying function definitions to enable their use with `constexpr`. Do not use `constexpr` to force inlining.

### Integer Types

Of the built-in C++ integer types, the only one used is `int`. If a program needs a variable of a different size, use a precise-width integer type from `<cstdint>`, such as `int16_t`. If your variable represents a value that could ever be greater than or equal to 2^31 (2GiB), use a 64-bit type such as `int64_t`. Keep in mind that even if your value won't ever be too large for an `int`, it may be used in intermediate calculations which may require a larger type. When in doubt, choose a larger type.

#### definition

C++ does not specify the sizes of integer types like `int`. Typically people assume that `short` is 16 bits, `int` is 32 bits, `long` is 32 bits and `long long` is 64 bits.

#### pros

Uniformity of declaration.

#### cons

The sizes of integral types in C++ can vary based on compiler and architecture.

#### decision

`<cstdint>` defines types like `int16_t`, `uint32_t`, `int64_t`, etc. You should always use those in preference to `short`, `unsigned long long` and the like, when you need a guarantee on the size of an integer. Of the C integer types, only `int` should be used. When appropriate, you are welcome to use standard types like `size_t` and `ptrdiff_t`.

We use `int` very often, for integers we know are not going to be too big, e.g., loop counters. Use plain old `int` for such things. You should assume that an `int` is at least 32 bits, but don't assume that it has more than 32 bits. If you need a 64-bit integer type, use `int64_t` or `uint64_t`.

For integers we know can be "big", use `int64_t`.

You should not use the unsigned integer types such as `uint32_t`, unless there is a valid reason such as representing a bit pattern rather than a number, or you need defined overflow modulo 2^N. In particular, do not use unsigned types to say a number will never be negative. Instead, use assertions for this.

If your code is a container that returns a size, be sure to use a type that will accommodate any possible usage of your container. When in doubt, use a larger type rather than a smaller type.

Use care when converting integer types. Integer conversions and promotions can cause undefined behavior, leading to security bugs and other problems.

#### stylepoint_subsection

#### On Unsigned Integers

Unsigned integers are good for representing bitfields and modular arithmetic. Because of historical accident, the C++ standard also uses unsigned integers to represent the size of containers - many members of the standards body believe this to be a mistake, but it is effectively impossible to fix at this point. The fact that unsigned arithmetic doesn't model the behavior of a simple integer, but is instead defined by the standard to model modular arithmetic (wrapping around on overflow/underflow), means that a significant class of bugs cannot be diagnosed by the compiler. In other cases, the defined behavior impedes optimization.

That said, mixing signedness of integer types is responsible for an equally large class of problems. The best advice we can provide: try to use iterators and containers rather than pointers and sizes, try not to mix signedness, and try to avoid unsigned types (except for representing bitfields or modular arithmetic). Do not use an unsigned type merely to assert that a variable is non-negative.

### 64-bit Portability

Code should be 64-bit and 32-bit friendly. Bear in mind problems of printing, comparisons, and structure alignment.

*   Correct portable `printf()` conversion specifiers for some integral typedefs rely on macro expansions that we find unpleasant to use and impractical to require (the `PRI` macros from `<cinttypes>`). Unless there is no reasonable alternative for your particular case, try to avoid or even upgrade APIs that rely on the `printf` family. Instead use a library supporting typesafe numeric formatting, such as [`StrCat`](https://github.com/abseil/abseil-cpp/blob/master/absl/strings/str_cat.h) or [`Substitute`](https://github.com/abseil/abseil-cpp/blob/master/absl/strings/substitute.h) for fast simple conversions, or [`std::ostream`](#Streams).

    Unfortunately, the `PRI` macros are the only portable way to specify a conversion for the standard bitwidth typedefs (e.g. `int64_t`, `uint64_t`, `int32_t`, `uint32_t`, etc). Where possible, avoid passing arguments of types specified by bitwidth typedefs to `printf`-based APIs. Note that it is acceptable to use typedefs for which printf has dedicated length modifiers, such as `size_t` (`z`), `ptrdiff_t` (`t`), and `maxint_t` (`j`).

*   Remember that `sizeof(void *)` != `sizeof(int)`. Use `intptr_t` if you want a pointer-sized integer.

*   You may need to be careful with structure alignments, particularly for structures being stored on disk. Any class/structure with a `int64_t`/`uint64_t` member will by default end up being 8-byte aligned on a 64-bit system. If you have such structures being shared on disk between 32-bit and 64-bit code, you will need to ensure that they are packed the same on both architectures. Most compilers offer a way to alter structure alignment. For gcc, you can use `__attribute__((packed))`. MSVC offers `#pragma pack()` and `__declspec(align())`.

*   Use [braced-initialization](#Casting) as needed to create 64-bit constants. For example:

    ```C++

int64_t my_value{0x123456789};

    uint64_t my_mask{3ULL << 48};

    ```

### Preprocessor Macros

Avoid defining macros, especially in headers; prefer inline functions, enums, and `const` variables. Name macros with a project-specific prefix. Do not use macros to define pieces of a C++ API.

Macros mean that the code you see is not the same as the code the compiler sees. This can introduce unexpected behavior, especially since macros have global scope.

The problems introduced by macros are especially severe when they are used to define pieces of a C++ API, and still more so for public APIs. Every error message from the compiler when developers incorrectly use that interface now must explain how the macros formed the interface. Refactoring and analysis tools have a dramatically harder time updating the interface. As a consequence, we specifically disallow using macros in this way. For example, avoid patterns like:

#### badcodeclass WOMBAT_TYPE(Foo) {

  // ...

 public:

  EXPAND_PUBLIC_WOMBAT_API(Foo)

  EXPAND_WOMBAT_COMPARISONS(Foo, ==, <)

};

```

Luckily, macros are not nearly as necessary in C++ as they are in C. Instead of using a macro to inline performance-critical code, use an inline function. Instead of using a macro to store a constant, use a `const` variable. Instead of using a macro to "abbreviate" a long variable name, use a reference. Instead of using a macro to conditionally compile code ... well, don't do that at all (except, of course, for the `#define` guards to prevent double inclusion of header files). It makes testing much more difficult.

Macros can do things these other techniques cannot, and you do see them in the codebase, especially in the lower-level libraries. And some of their special features (like stringifying, concatenation, and so forth) are not available through the language proper. But before using a macro, consider carefully whether there's a non-macro way to achieve the same result. If you need to use a macro to define an interface, contact your project leads to request a waiver of this rule.

The following usage pattern will avoid many problems with macros; if you use macros, follow it whenever possible:

*   Don't define macros in a `.h` file.

*   `#define` macros right before you use them, and `#undef` them right after.

*   Do not just `#undef` an existing macro before replacing it with your own; instead, pick a name that's likely to be unique.

*   Try not to use macros that expand to unbalanced C++ constructs, or at least document that behavior well.

*   Prefer not using `##` to generate function/class/variable names.

Exporting macros from headers (i.e. defining them in a header without `#undef`ing them before the end of the header) is extremely strongly discouraged. If you do export a macro from a header, it must have a globally unique name. To achieve this, it must be named with a prefix consisting of your project's namespace name (but upper case).

### 0 and nullptr/NULL

Use `0` for integers, `0.0` for reals, `nullptr` for pointers, and `'\0'` for chars.

Use `0` for integers and `0.0` for reals.

For pointers (address values), use `nullptr`, as this provides type-safety.

For C++03 projects, prefer `NULL` to `0`. While the values are equivalent, `NULL` looks more like a pointer to the reader, and some C++ compilers provide special definitions of `NULL` which enable them to give useful warnings.

Use `'\0'` for the null character. Using the correct type makes the code more readable.

### sizeof

Prefer `sizeof(<var>varname</var>)` to `sizeof(<var>type</var>)`.

Use `sizeof(<var>varname</var>)` when you take the size of a particular variable. `sizeof(<var>varname</var>)` will update appropriately if someone changes the variable type either now or later. You may use `sizeof(<var>type</var>)` for code unrelated to any particular variable, such as code that manages an external or internal data format where a variable of an appropriate C++ type is not convenient.

```C++

Struct data;

memset(&data, 0, sizeof(data));

```

#### badcodememset(&data, 0, sizeof(Struct));

```

```C++

if (raw_size < sizeof(int)) {

  LOG(ERROR) << "compressed record not big enough for count: " << raw_size;

  return false;

}

```

### auto

Use `auto` to avoid type names that are noisy, obvious, or unimportant - cases where the type doesn't aid in clarity for the reader. Continue to use manifest type declarations when it helps readability.

#### pros

*   C++ type names can be long and cumbersome, especially when they involve templates or namespaces.

*   When a C++ type name is repeated within a single declaration or a small code region, the repetition may not be aiding readability.

*   It is sometimes safer to let the type be specified by the type of the initialization expression, since that avoids the possibility of unintended copies or type conversions.

#### cons

Sometimes code is clearer when types are manifest, especially when a variable's initialization depends on things that were declared far away. In expressions like:

#### badcodeauto foo = x.add_foo();

auto i = y.Find(key);

```

it may not be obvious what the resulting types are if the type of `y` isn't very well known, or if `y` was declared many lines earlier.

Programmers have to understand the difference between `auto` and `const auto&` or they'll get copies when they didn't mean to.

If an `auto` variable is used as part of an interface, e.g. as a constant in a header, then a programmer might change its type while only intending to change its value, leading to a more radical API change than intended.

#### decision

`auto` is permitted when it increases readability, particularly as described below. Never initialize an `auto`-typed variable with a braced initializer list.

Specific cases where `auto` is allowed or encouraged:

*   (Encouraged) For iterators and other long/cluttery type names, particularly when the type is clear from context (calls to `find`, `begin`, or `end` for instance).

*   (Allowed) When the type is clear from local context (in the same expression or within a few lines). Initialization of a pointer or smart pointer with calls to `new` and `std::make_unique` commonly falls into this category, as does use of `auto` in a range-based loop over a container whose type is spelled out nearby.

*   (Allowed) When the type doesn't matter because it isn't being used for anything other than equality comparison.

*   (Encouraged) When iterating over a map with a range-based loop (because it is often assumed that the correct type is `std::pair<KeyType, ValueType>` whereas it is actually `std::pair<const KeyType, ValueType>`). This is particularly well paired with local `key` and `value` aliases for `.first` and `.second` (often const-ref).

    #### codefor (const auto& item : some_map) {

      const KeyType& key = item.first;

      const ValType& value = item.second;

      // The rest of the loop can now just refer to key and value,

      // a reader can see the types in question, and we've avoided

      // the too-common case of extra copies in this iteration.

    }

    ```

### Braced Initializer List

You may use braced initializer lists.

In C++03, aggregate types (arrays and structs with no constructor) could be initialized with braced initializer lists.

```C++

struct Point { int x; int y; };

Point p = {1, 2};

```

In C++11, this syntax was generalized, and any object type can now be created with a braced initializer list, known as a _braced-init-list_ in the C++ grammar. Here are a few examples of its use.

```C++

// Vector takes a braced-init-list of elements.

std::vector<string> v{"foo", "bar"};

// Basically the same, ignoring some small technicalities.

// You may choose to use either form.

std::vector<string> v = {"foo", "bar"};

// Usable with 'new' expressions.

auto p = new std::vector<string>{"foo", "bar"};

// A map can take a list of pairs. Nested braced-init-lists work.

std::map<int, string> m = {{1, "one"}, {2, "2"}};

// A braced-init-list can be implicitly converted to a return type.

std::vector<int> test_function() { return {1, 2, 3}; }

// Iterate over a braced-init-list.

for (int i : {-1, -2, -3}) {}

// Call a function using a braced-init-list.

void TestFunction2(std::vector<int> v) {}

TestFunction2({1, 2, 3});

```

A user-defined type can also define a constructor and/or assignment operator that take `std::initializer_list<T>`, which is automatically created from _braced-init-list_:

```C++

class MyType {

 public:

  // std::initializer_list references the underlying init list.

  // It should be passed by value.

  MyType(std::initializer_list<int> init_list) {

    for (int i : init_list) append(i);

  }

  MyType& operator=(std::initializer_list<int> init_list) {

    clear();

    for (int i : init_list) append(i);

  }

};

MyType m{2, 3, 5, 7};

```

Finally, brace initialization can also call ordinary constructors of data types, even if they do not have `std::initializer_list<T>` constructors.

```C++

double d{1.23};

// Calls ordinary constructor as long as MyOtherType has no

// std::initializer_list constructor.

class MyOtherType {

 public:

  explicit MyOtherType(string);

  MyOtherType(int, string);

};

MyOtherType m = {1, "b"};

// If the constructor is explicit, you can't use the "= {}" form.

MyOtherType m{"b"};

```

Never assign a _braced-init-list_ to an auto local variable. In the single element case, what this means can be confusing.

#### badcodeauto d = {1.23};        // d is a std::initializer_list<double>

```

```C++

auto d = double{1.23};  // Good -- d is a double, not a std::initializer_list.

```

See [Braced_Initializer_List_Format](#Braced_Initializer_List_Format) for formatting.

### Lambda expressions

Use lambda expressions where appropriate. Prefer explicit captures when the lambda will escape the current scope.

#### definition

Lambda expressions are a concise way of creating anonymous function objects. They're often useful when passing functions as arguments. For example:

```C++

std::sort(v.begin(), v.end(), [](int x, int y) {

  return Weight(x) < Weight(y);

});

```

They further allow capturing variables from the enclosing scope either explicitly by name, or implicitly using a default capture. Explicit captures require each variable to be listed, as either a value or reference capture:

```C++

int weight = 3;

int sum = 0;

// Captures `weight` by value and `sum` by reference.

std::for_each(v.begin(), v.end(), [weight, &sum](int x) {

  sum += weight * x;

});

```

Default captures implicitly capture any variable referenced in the lambda body, including `this` if any members are used:

```C++

const std::vector<int> lookup_table = ...;

std::vector<int> indices = ...;

// Captures `lookup_table` by reference, sorts `indices` by the value

// of the associated element in `lookup_table`.

std::sort(indices.begin(), indices.end(), [&](int a, int b) {

  return lookup_table[a] < lookup_table[b];

});

```

Lambdas were introduced in C++11 along with a set of utilities for working with function objects, such as the polymorphic wrapper `std::function`.

#### pros

*   Lambdas are much more concise than other ways of defining function objects to be passed to STL algorithms, which can be a readability improvement.

*   Appropriate use of default captures can remove redundancy and highlight important exceptions from the default.

*   Lambdas, `std::function`, and `std::bind` can be used in combination as a general purpose callback mechanism; they make it easy to write functions that take bound functions as arguments.

#### cons

*   Variable capture in lambdas can be a source of dangling-pointer bugs, particularly if a lambda escapes the current scope.

*   Default captures by value can be misleading because they do not prevent dangling-pointer bugs. Capturing a pointer by value doesn't cause a deep copy, so it often has the same lifetime issues as capture by reference. This is especially confusing when capturing 'this' by value, since the use of 'this' is often implicit.

*   It's possible for use of lambdas to get out of hand; very long nested anonymous functions can make code harder to understand.

#### decision

*   Use lambda expressions where appropriate, with formatting as described [below](#Formatting_Lambda_Expressions).

*   Prefer explicit captures if the lambda may escape the current scope. For example, instead of:

    #### badcode{

      Foo foo;

      ...

      executor->Schedule([&] { Frobnicate(foo); })

      ...

    }

    // BAD! The fact that the lambda makes use of a reference to `foo` and

    // possibly `this` (if `Frobnicate` is a member function) may not be

    // apparent on a cursory inspection. If the lambda is invoked after

    // the function returns, that would be bad, because both `foo`

    // and the enclosing object could have been destroyed.

    ```

    prefer to write:

    ```C++

{

      Foo foo;

      ...

      executor->Schedule([&foo] { Frobnicate(foo); })

      ...

    }

    // BETTER - The compile will fail if `Frobnicate` is a member

    // function, and it's clearer that `foo` is dangerously captured by

    // reference.

    ```

*   Use default capture by reference ([&]) only when the lifetime of the lambda is obviously shorter than any potential captures.

*   Use default capture by value ([=]) only as a means of binding a few variables for a short lambda, where the set of captured variables is obvious at a glance. Prefer not to write long or complex lambdas with default capture by value.

*   Specify the return type of the lambda explicitly if that will make it more obvious to readers, as with [`auto`](#auto).

### Template metaprogramming

Avoid complicated template programming.

#### definition

Template metaprogramming refers to a family of techniques that exploit the fact that the C++ template instantiation mechanism is Turing complete and can be used to perform arbitrary compile-time computation in the type domain.

#### pros

Template metaprogramming allows extremely flexible interfaces that are type safe and high performance. Facilities like [Google Test](https://code.google.com/p/googletest/), `std::tuple`, `std::function`, and Boost.Spirit would be impossible without it.

#### cons

The techniques used in template metaprogramming are often obscure to anyone but language experts. Code that uses templates in complicated ways is often unreadable, and is hard to debug or maintain.

Template metaprogramming often leads to extremely poor compiler time error messages: even if an interface is simple, the complicated implementation details become visible when the user does something wrong.

Template metaprogramming interferes with large scale refactoring by making the job of refactoring tools harder. First, the template code is expanded in multiple contexts, and it's hard to verify that the transformation makes sense in all of them. Second, some refactoring tools work with an AST that only represents the structure of the code after template expansion. It can be difficult to automatically work back to the original source construct that needs to be rewritten.

#### decision

Template metaprogramming sometimes allows cleaner and easier-to-use interfaces than would be possible without it, but it's also often a temptation to be overly clever. It's best used in a small number of low level components where the extra maintenance burden is spread out over a large number of uses.

Think twice before using template metaprogramming or other complicated template techniques; think about whether the average member of your team will be able to understand your code well enough to maintain it after you switch to another project, or whether a non-C++ programmer or someone casually browsing the code base will be able to understand the error messages or trace the flow of a function they want to call. If you're using recursive template instantiations or type lists or metafunctions or expression templates, or relying on SFINAE or on the `sizeof` trick for detecting function overload resolution, then there's a good chance you've gone too far.

If you use template metaprogramming, you should expect to put considerable effort into minimizing and isolating the complexity. You should hide metaprogramming as an implementation detail whenever possible, so that user-facing headers are readable, and you should make sure that tricky code is especially well commented. You should carefully document how the code is used, and you should say something about what the "generated" code looks like. Pay extra attention to the error messages that the compiler emits when users make mistakes. The error messages are part of your user interface, and your code should be tweaked as necessary so that the error messages are understandable and actionable from a user point of view.

### Boost

Use only approved libraries from the Boost library collection.

#### definition

The [Boost library collection](https://www.boost.org/) is a popular collection of peer-reviewed, free, open-source C++ libraries.

#### pros

Boost code is generally very high-quality, is widely portable, and fills many important gaps in the C++ standard library, such as type traits and better binders.

#### cons

Some Boost libraries encourage coding practices which can hamper readability, such as metaprogramming and other advanced template techniques, and an excessively "functional" style of programming.

#### decision

<div>

In order to maintain a high level of readability for all contributors who might read and maintain code, we only allow an approved subset of Boost features. Currently, the following libraries are permitted:

*   [Call Traits](https://www.boost.org/libs/utility/call_traits.htm) from `boost/call_traits.hpp`

*   [Compressed Pair](https://www.boost.org/libs/utility/compressed_pair.htm) from `boost/compressed_pair.hpp`

*   [The Boost Graph Library (BGL)](https://www.boost.org/libs/graph/) from `boost/graph`, except serialization (`adj_list_serialize.hpp`) and parallel/distributed algorithms and data structures (`boost/graph/parallel/*` and `boost/graph/distributed/*`).

*   [Property Map](https://www.boost.org/libs/property_map/) from `boost/property_map`, except parallel/distributed property maps (`boost/property_map/parallel/*`).

*   [Iterator](https://www.boost.org/libs/iterator/) from `boost/iterator`

*   The part of [Polygon](https://www.boost.org/libs/polygon/) that deals with Voronoi diagram construction and doesn't depend on the rest of Polygon: `boost/polygon/voronoi_builder.hpp`, `boost/polygon/voronoi_diagram.hpp`, and `boost/polygon/voronoi_geometry_type.hpp`

*   [Bimap](https://www.boost.org/libs/bimap/) from `boost/bimap`

*   [Statistical Distributions and Functions](https://www.boost.org/libs/math/doc/html/dist.html) from `boost/math/distributions`

*   [Special Functions](https://www.boost.org/libs/math/doc/html/special.html) from `boost/math/special_functions`

*   [Multi-index](https://www.boost.org/libs/multi_index/) from `boost/multi_index`

*   [Heap](https://www.boost.org/libs/heap/) from `boost/heap`

*   The flat containers from [Container](https://www.boost.org/libs/container/): `boost/container/flat_map`, and `boost/container/flat_set`

*   [Intrusive](https://www.boost.org/libs/intrusive/) from `boost/intrusive`.

*   [The `boost/sort` library](https://www.boost.org/libs/sort/).

*   [Preprocessor](https://www.boost.org/libs/preprocessor/) from `boost/preprocessor`.

We are actively considering adding other Boost features to the list, so this list may be expanded in the future.

The following libraries are permitted, but their use is discouraged because they've been superseded by standard libraries in C++11:

*   [Array](https://www.boost.org/libs/array/) from `boost/array.hpp`: use [`std::array`](http://en.cppreference.com/w/cpp/container/array) instead.

*   [Pointer Container](https://www.boost.org/libs/ptr_container/) from `boost/ptr_container`: use containers of [`std::unique_ptr`](http://en.cppreference.com/w/cpp/memory/unique_ptr) instead.

### std::hash

Do not define specializations of `std::hash`.

#### definition

`std::hash<T>` is the function object that the C++11 hash containers use to hash keys of type `T`, unless the user explicitly specifies a different hash function. For example, `std::unordered_map<int, string>` is a hash map that uses `std::hash<int>` to hash its keys, whereas `std::unordered_map<int, string, MyIntHash>` uses `MyIntHash`.

`std::hash` is defined for all integral, floating-point, pointer, and `enum` types, as well as some standard library types such as `string` and `unique_ptr`. Users can enable it to work for their own types by defining specializations of it for those types.

#### pros

`std::hash` is easy to use, and simplifies the code since you don't have to name it explicitly. Specializing `std::hash` is the standard way of specifying how to hash a type, so it's what outside resources will teach, and what new engineers will expect.

#### cons

`std::hash` is hard to specialize. It requires a lot of boilerplate code, and more importantly, it combines responsibility for identifying the hash inputs with responsibility for executing the hashing algorithm itself. The type author has to be responsible for the former, but the latter requires expertise that a type author usually doesn't have, and shouldn't need. The stakes here are high because low-quality hash functions can be security vulnerabilities, due to the emergence of [hash flooding attacks](https://emboss.github.io/blog/2012/12/14/breaking-murmur-hash-flooding-dos-reloaded/).

Even for experts, `std::hash` specializations are inordinately difficult to implement correctly for compound types, because the implementation cannot recursively call `std::hash` on data members. High-quality hash algorithms maintain large amounts of internal state, and reducing that state to the `size_t` bytes that `std::hash` returns is usually the slowest part of the computation, so it should not be done more than once.

Due to exactly that issue, `std::hash` does not work with `std::pair` or `std::tuple`, and the language does not allow us to extend it to support them.

#### decision

You can use `std::hash` with the types that it supports "out of the box", but do not specialize it to support additional types. If you need a hash table with a key type that `std::hash` does not support, consider using legacy hash containers (e.g. `hash_map`) for now; they use a different default hasher, which is unaffected by this prohibition.

If you want to use the standard hash containers anyway, you will need to specify a custom hasher for the key type, e.g.

```C++

std::unordered_map<MyKeyType, Value, MyKeyTypeHasher> my_map;

```

Consult with the type's owners to see if there is an existing hasher that you can use; otherwise work with them to provide one, or roll your own.

We are planning to provide a hash function that can work with any type, using a new customization mechanism that doesn't have the drawbacks of `std::hash`.

### C++11

Use libraries and language extensions from C++11 when appropriate. Consider portability to other environments before using C++11 features in your project.

#### definition

C++11 contains [significant changes](https://en.wikipedia.org/wiki/C%2B%2B11) both to the language and libraries.

#### pros

C++11 was the official standard until 2014, and is supported by most C++ compilers. It standardizes some common C++ extensions that we use already, allows shorthands for some operations, and has some performance and safety improvements.

#### cons

The C++11 standard is substantially more complex than its predecessor (1,300 pages versus 800 pages), and is unfamiliar to many developers. The long-term effects of some features on code readability and maintenance are unknown. We cannot predict when its various features will be implemented uniformly by tools that may be of interest, particularly in the case of projects that are forced to use older versions of tools.

As with [Boost](#Boost), some C++11 extensions encourage coding practices that hamper readability—for example by removing checked redundancy (such as type names) that may be helpful to readers, or by encouraging template metaprogramming. Other extensions duplicate functionality available through existing mechanisms, which may lead to confusion and conversion costs.

#### decision

C++11 features may be used unless specified otherwise. In addition to what's described in the rest of the style guide, the following C++11 features may not be used:

*   Compile-time rational numbers (`<ratio>`), because of concerns that it's tied to a more template-heavy interface style.

*   The `<cfenv>` and `<fenv.h>` headers, because many compilers do not support those features reliably.

### Nonstandard Extensions

Nonstandard extensions to C++ may not be used unless otherwise specified.

#### definition

Compilers support various extensions that are not part of standard C++. Such extensions include GCC's `__attribute__`, intrinsic functions such as `__builtin_prefetch`, designated initializers (e.g. `Foo f = {.field = 3}`), inline assembly, `__COUNTER__`, `__PRETTY_FUNCTION__`, compound statement expressions (e.g. `foo = ({ int x; Bar(&x); x })`, variable-length arrays and `alloca()`, and the "[Elvis Operator](https://en.wikipedia.org/wiki/Elvis_operator)" `a?:b`.

#### pros

*   Nonstandard extensions may provide useful features that do not exist in standard C++. For example, some people think that designated initializers are more readable than standard C++ features like constructors.

*   Important performance guidance to the compiler can only be specified using extensions.

#### cons

*   Nonstandard extensions do not work in all compilers. Use of nonstandard extensions reduces portability of code.

*   Even if they are supported in all targeted compilers, the extensions are often not well-specified, and there may be subtle behavior differences between compilers.

*   Nonstandard extensions add to the language features that a reader must know to understand the code.

#### decision

Do not use nonstandard extensions. You may use portability wrappers that are implemented using nonstandard extensions, so long as those wrappers are provided by a designated project-wide portability header.

### Aliases

Public aliases are for the benefit of an API's user, and should be clearly documented.

#### definition

There are several ways to create names that are aliases of other entities:

```C++

typedef Foo Bar;

using Bar = Foo;

using other_namespace::Foo;

```

In new code, `using` is preferable to `typedef`, because it provides a more consistent syntax with the rest of C++ and works with templates.

Like other declarations, aliases declared in a header file are part of that header's public API unless they're in a function definition, in the private portion of a class, or in an explicitly-marked internal namespace. Aliases in such areas or in .cc files are implementation details (because client code can't refer to them), and are not restricted by this rule.

#### pros

*   Aliases can improve readability by simplifying a long or complicated name.

*   Aliases can reduce duplication by naming in one place a type used repeatedly in an API, which _might_ make it easier to change the type later.

#### cons

*   When placed in a header where client code can refer to them, aliases increase the number of entities in that header's API, increasing its complexity.

*   Clients can easily rely on unintended details of public aliases, making changes difficult.

*   It can be tempting to create a public alias that is only intended for use in the implementation, without considering its impact on the API, or on maintainability.

*   Aliases can create risk of name collisions

*   Aliases can reduce readability by giving a familiar construct an unfamiliar name

*   Type aliases can create an unclear API contract: it is unclear whether the alias is guaranteed to be identical to the type it aliases, to have the same API, or only to be usable in specified narrow ways

#### decision

Don't put an alias in your public API just to save typing in the implementation; do so only if you intend it to be used by your clients.

When defining a public alias, document the intent of the new name, including whether it is guaranteed to always be the same as the type it's currently aliased to, or whether a more limited compatibility is intended. This lets the user know whether they can treat the types as substitutable or whether more specific rules must be followed, and can help the implementation retain some degree of freedom to change the alias.

Don't put namespace aliases in your public API. (See also [Namespaces](#Namespaces)).

For example, these aliases document how they are intended to be used in client code:

```C++

namespace mynamespace {

// Used to store field measurements. DataPoint may change from Bar* to some internal type.

// Client code should treat it as an opaque pointer.

using DataPoint = foo::Bar*;

// A set of measurements. Just an alias for user convenience.

using TimeSeries = std::unordered_set<DataPoint, std::hash<DataPoint>, DataPointComparator>;

}  // namespace mynamespace

```

These aliases don't document intended use, and half of them aren't meant for client use:

#### badcodenamespace mynamespace {

// Bad: none of these say how they should be used.

using DataPoint = foo::Bar*;

using std::unordered_set;  // Bad: just for local convenience

using std::hash;           // Bad: just for local convenience

typedef unordered_set<DataPoint, hash<DataPoint>, DataPointComparator> TimeSeries;

}  // namespace mynamespace

```

However, local convenience aliases are fine in function definitions, private sections of classes, explicitly marked internal namespaces, and in .cc files:

```C++

// In a .cc file

using foo::Bar;

```
