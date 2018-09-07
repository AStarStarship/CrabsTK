F2 Foundational Framework C++ Style Guide
=========================================

## Naming

The most important consistency rules are those that govern naming. The style of a name immediately informs us what sort of thing the named entity is: a type, a variable, a function, a constant, a macro, etc., without requiring us to search for the declaration of that entity. The pattern-matching engine in our brains relies a great deal on these naming rules.

Naming rules are pretty arbitrary, but we feel that consistency is more important than individual preferences in this area, so regardless of whether you find them sensible or not, the rules are the rules.

### General Naming Rules

#### summary

Names should be descriptive; avoid abbreviation.

Give as descriptive a name as possible, within reason. Do not worry about saving horizontal space as it is far more important to make your code immediately understandable by a new reader. Do not use abbreviations that are ambiguous or unfamiliar to readers outside your project, and do not abbreviate by deleting letters within a word. Abbreviations that would be familiar to someone outside your project with relevant domain knowledge are OK. As a rule of thumb, an abbreviation is probably OK if it's listed in Wikipedia.

```C++

int price_count_reader;    // No abbreviation.

int num_errors;            // "num" is a widespread convention.

int num_dns_connections;   // Most people know what "DNS" stands for.

int lstm_size;             // "LSTM" is a common machine learning abbreviation.

```

#### badcodeint n;                     // Meaningless.

int nerr;                  // Ambiguous abbreviation.

int n_comp_conns;          // Ambiguous abbreviation.

int wgc_connections;       // Only your group knows what this stands for.

int pc_reader;             // Lots of things can be abbreviated "pc".

int cstmr_id;              // Deletes internal letters.

FooBarRequestInfo fbri;    // Not even a word.

```

Note that certain universally-known abbreviations are OK, such as `i` for an iteration variable and `T` for a template parameter.

For some symbols, this style guide recommends names to start with a capital letter and to have a capital letter for each new word (a.k.a. "[Camel Case](https://en.wikipedia.org/wiki/Camel_case)" or "Pascal case"). When abbreviations or acronyms appear in such names, prefer to capitalize the abbreviations or acronyms as single words (i.e `StartRpc()`, not `StartRPC()`).

Template parameters should follow the naming style for their category: type template parameters should follow the rules for [type names](#Type_Names), and non-type template parameters should follow the rules for [variable names](#Variable_Names).

### File Names

#### summary

Filenames should be all lowercase and can include underscores (`_`) or dashes (`-`). Follow the convention that your project uses. If there is no consistent local pattern to follow, prefer "_".

Examples of acceptable file names:

*   `my_useful_class.cc`

*   `my-useful-class.cc`

*   `myusefulclass.cc`

*   `myusefulclass_test.cc // _unittest and _regtest are deprecated.`

C++ files should end in `.cc` and header files should end in `.h`. Files that rely on being textually included at specific points should end in `.inc` (see also the section on [self-contained headers](#Self_contained_Headers)).

Do not use filenames that already exist in `/usr/include`, such as `db.h`.

In general, make your filenames very specific. For example, use `http_server_logs.h` rather than `logs.h`. A very common case is to have a pair of files called, e.g., `foo_bar.h` and `foo_bar.cc`, defining a class called `FooBar`.

### Type Names

#### summary

Type names start with a capital letter and have a capital letter for each new word, with no underscores: `MyExcitingClass`, `MyExcitingEnum`.

The names of all types — classes, structs, type aliases, enums, and type template parameters — have the same naming convention. Type names should start with a capital letter and have a capital letter for each new word. No underscores. For example:

```C++

// classes and structs

class UrlTable { ...

class UrlTableTester { ...

struct UrlTableProperties { ...

// typedefs

typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using aliases

using PropertiesMap = hash_map<UrlTableProperties *, string>;

// enums

enum UrlTableErrors { ...

```

### Variable Names

#### summary

The names of variables (including function parameters) and data members are all lowercase, with underscores between words. Data members of classes (but not structs) additionally have trailing underscores. For instance: `a_local_variable`, `a_struct_data_member`, `a_class_data_member_`.

#### Common Variable names

For example:

```C++

string table_name;  // OK - uses underscore.

string tablename;   // OK - all lowercase.

```

#### badcodestring tableName;   // Bad - mixed case.

```

#### Class Data Members

Data members of classes, both static and non-static, are named like ordinary nonmember variables, but with a trailing underscore.

```C++

class TableInfo {

  ...

 private:

  string table_name_;  // OK - underscore at end.

  string tablename_;   // OK.

  static Pool<TableInfo>* pool_;  // OK.

};

```

#### Struct Data Members

Data members of structs, both static and non-static, are named like ordinary nonmember variables. They do not have the trailing underscores that data members in classes have.

```C++

struct UrlTableProperties {

  string name;

  int num_entries;

  static Pool<UrlTableProperties>* pool;

};

```

See [Structs vs. Classes](#Structs_vs._Classes) for a discussion of when to use a struct versus a class.

### Constant Names

#### summary

Variables declared constexpr or const, and whose value is fixed for the duration of the program, are named with a leading "k" followed by mixed case. Underscores can be used as separators in the rare cases where capitalization cannot be used for separation. For example:

```C++

const int kDaysInAWeek = 7;

const int kAndroid8_0_0 = 24;  // Android 8.0.0

```

All such variables with static storage duration (i.e. statics and globals, see [Storage Duration](http://en.cppreference.com/w/cpp/language/storage_duration#Storage_duration) for details) should be named this way. This convention is optional for variables of other storage classes, e.g. automatic variables, otherwise the usual variable naming rules apply.

### Function Names

#### summary

Regular functions have mixed case; accessors and mutators may be named like variables.

Ordinarily, functions should start with a capital letter and have a capital letter for each new word.

```C++

AddTableEntry()

DeleteUrl()

OpenFileOrDie()

```

(The same naming rule applies to class- and namespace-scope constants that are exposed as part of an API and that are intended to look like functions, because the fact that they're objects rather than functions is an unimportant implementation detail.)

Accessors and mutators (get and set functions) may be named like variables. These often correspond to actual member variables, but this is not required. For example, `int count()` and `void set_count(int count)`.

### Namespace Names

#### summaryNamespace names are all lower-case. Top-level namespace names are based on the project name . Avoid collisions between nested namespaces and well-known top-level namespaces.

The name of a top-level namespace should usually be the name of the project or team whose code is contained in that namespace. The code in that namespace should usually be in a directory whose basename matches the namespace name (or in subdirectories thereof).

Keep in mind that the [rule against abbreviated names](#General_Naming_Rules) applies to namespaces just as much as variable names. Code inside the namespace seldom needs to mention the namespace name, so there's usually no particular need for abbreviation anyway.

Avoid nested namespaces that match well-known top-level namespaces. Collisions between namespace names can lead to surprising build breaks because of name lookup rules. In particular, do not create any nested `std` namespaces. Prefer unique project identifiers (`websearch::index`, `websearch::index_util`) over collision-prone names like `websearch::util`.

For `internal` namespaces, be wary of other code being added to the same `internal` namespace causing a collision (internal helpers within a team tend to be related and may lead to collisions). In such a situation, using the filename to make a unique internal name is helpful (`websearch::index::frobber_internal` for use in `frobber.h`)

### Enumerator Names

#### summary

Enumerators (for both scoped and unscoped enums) should be named _either_ like [constants](#Constant_Names) or like [macros](#Macro_Names): either `kEnumName` or `ENUM_NAME`.

Preferably, the individual enumerators should be named like [constants](#Constant_Names). However, it is also acceptable to name them like [macros](#Macro_Names). The enumeration name, `UrlTableErrors` (and `AlternateUrlTableErrors`), is a type, and therefore mixed case.

```C++

enum UrlTableErrors {

  kOK = 0,

  kErrorOutOfMemory,

  kErrorMalformedInput,

};

enum AlternateUrlTableErrors {

  OK = 0,

  OUT_OF_MEMORY = 1,

  MALFORMED_INPUT = 2,

};

```

Until January 2009, the style was to name enum values like [macros](#Macro_Names). This caused problems with name collisions between enum values and macros. Hence, the change to prefer constant-style naming was put in place. New code should prefer constant-style naming if possible. However, there is no reason to change old code to use constant-style names, unless the old names are actually causing a compile-time problem.

### Macro Names

#### summary

You're not really going to [define a macro](#Preprocessor_Macros), are you? If you do, they're like this: `MY_MACRO_THAT_SCARES_SMALL_CHILDREN_AND_ADULTS_ALIKE`.

Please see the [description of macros](#Preprocessor_Macros); in general macros should _not_ be used. However, if they are absolutely needed, then they should be named with all capitals and underscores.

```C++

#define ROUND(x) ...

#define PI_ROUNDED 3.0

```

### Exceptions to Naming Rules

#### summary

If you are naming something that is analogous to an existing C or C++ entity then you can follow the existing naming convention scheme.

<dl>

* `bigopen()`

** function name, follows form of `open()`

* `uint`

** `typedef`

* `bigpos`

** `struct` or `class`, follows form of `pos`

* `sparse_hash_map`

** STL-like entity; follows STL naming conventions

* `LONGLONG_MAX`

** a constant, as in `INT_MAX`

## Comments

Though a pain to write, comments are absolutely vital to keeping our code readable. The following rules describe what you should comment and where. But remember: while comments are very important, the best code is self-documenting. Giving sensible names to types and variables is much better than using obscure names that you must then explain through comments.

When writing your comments, write for your audience: the next contributor who will need to understand your code. Be generous — the next one may be you!
