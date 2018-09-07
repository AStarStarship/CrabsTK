F2 Foundational Framework C++ Style Guide
=========================================

### Comment Style

Use either the `//` or `/* */` syntax, as long as you are consistent.

You can use either the `//` or the `/* */` syntax; however, `//` is _much_ more common. Be consistent with how you comment and what style you use where.

### File Comments

Start each file with license boilerplate.

File comments describe the contents of a file. If a file declares, implements, or tests exactly one abstraction that is documented by a comment at the point of declaration, file comments are not required. All other files must have file comments.

#### Legal Notice and Author Line

Every file should contain license boilerplate. Choose the appropriate boilerplate for the license used by the project (for example, Apache 2.0, BSD, LGPL, GPL).

If you make significant changes to a file with an author line, consider deleting the author line. New files should usually not contain copyright notice or author line.

#### File Contents

If a `.h` declares multiple abstractions, the file-level comment should broadly describe the contents of the file, and how the abstractions are related. A 1 or 2 sentence file-level comment may be sufficient. The detailed documentation about individual abstractions belongs with those abstractions, not at the file level.

Do not duplicate comments in both the `.h` and the `.cc`. Duplicated comments diverge.

### Class Comments

Every non-obvious class declaration should have an accompanying comment that describes what it is for and how it should be used.

```C++

// Iterates over the contents of a GargantuanTable.

// Example:

//    GargantuanTableIterator* iter = table->NewIterator();

//    for (iter->Seek("foo"); !iter->done(); iter->Next()) {

//      process(iter->key(), iter->value());

//    }

//    delete iter;

class GargantuanTableIterator {

  ...

};

```

The class comment should provide the reader with enough information to know how and when to use the class, as well as any additional considerations necessary to correctly use the class. Document the synchronization assumptions the class makes, if any. If an instance of the class can be accessed by multiple threads, take extra care to document the rules and invariants surrounding multithreaded use.

The class comment is often a good place for a small example code snippet demonstrating a simple and focused usage of the class.

When sufficiently separated (e.g. `.h` and `.cc` files), comments describing the use of the class should go together with its interface definition; comments about the class operation and implementation should accompany the implementation of the class's methods.

### Function Comments

Declaration comments describe use of the function (when it is non-obvious); comments at the definition of a function describe operation.

#### Function Declarations

Almost every function declaration should have comments immediately preceding it that describe what the function does and how to use it. These comments may be omitted only if the function is simple and obvious (e.g. simple accessors for obvious properties of the class). These comments should be descriptive ("Opens the file") rather than imperative ("Open the file"); the comment describes the function, it does not tell the function what to do. In general, these comments do not describe how the function performs its task. Instead, that should be left to comments in the function definition.

Types of things to mention in comments at the function declaration:

*   What the inputs and outputs are.
*   For class member functions: whether the object remembers reference arguments beyond the duration of the method call, and whether it will free them or not.
*   If the function allocates memory that the caller must free.
*   Whether any of the arguments can be a null pointer.
*   If there are any performance implications of how a function is used.
*   If the function is re-entrant. What are its synchronization assumptions?

Here is an example:

```C++
// Returns an iterator for this table.  It is the client's
// responsibility to delete the iterator when it is done with it,
// and it must not use the iterator once the GargantuanTable object
// on which the iterator was created has been deleted.
//
// The iterator is initially positioned at the beginning of the table.
//
// This method is equivalent to:
//    Iterator* iter = table->NewIterator();
//    iter->Seek("");
//    return iter;
// If you are going to immediately seek to another place in the
// returned iterator, it will be faster to use NewIterator()
// and avoid the extra seek.

Iterator* GetIterator() const;
```

However, do not be unnecessarily verbose or state the completely obvious.

When documenting function overrides, focus on the specifics of the override itself, rather than repeating the comment from the overridden function. In many of these cases, the override needs no additional documentation and thus no comment is required.

When commenting constructors and destructors, remember that the person reading your code knows what constructors and destructors are for, so comments that just say something like "destroys this object" are not useful. Document what constructors do with their arguments (for example, if they take ownership of pointers), and what cleanup the destructor does. If this is trivial, just skip the comment. It is quite common for destructors not to have a header comment.

#### Function Definitions

If there is anything tricky about how a function does its job, the function definition should have an explanatory comment. For example, in the definition comment you might describe any coding tricks you use, give an overview of the steps you go through, or explain why you chose to implement the function in the way you did rather than using a viable alternative. For instance, you might mention why it must acquire a lock for the first half of the function but why it is not needed for the second half.

Note you should _not_ just repeat the comments given with the function declaration, in the `.h` file or wherever. It's okay to recapitulate briefly what the function does, but the focus of the comments should be on how it does it.

### Variable Comments

In general the actual name of the variable should be descriptive enough to give a good idea of what the variable is used for. In certain cases, more comments are required.

#### Class Data Members

The purpose of each class data member (also called an instance variable or member variable) must be clear. If there are any invariants (special values, relationships between members, lifetime requirements) not clearly expressed by the type and name, they must be commented. However, if the type and name suffice (`int num_events_;`), no comment is needed.

In particular, add comments to describe the existence and meaning of sentinel values, such as nullptr or -1, when they are not obvious. For example:

```C++

private:

 // Used to bounds-check table accesses. -1 means

 // that we don't yet know how many entries the table has.

 int num_total_entries_;

```

#### Global Variables

All global variables should have a comment describing what they are, what they are used for, and (if unclear) why it needs to be global. For example:

```C++

// The total number of tests cases that we run through in this regression test.

const int kNumTestCases = 6;

```

### Implementation Comments

In your implementation you should have comments in tricky, non-obvious, interesting, or important parts of your code.

#### Explanatory Comments

Tricky or complicated code blocks should have comments before them. Example:

```C++

// Divides result by two, taking into account that x

// contains the carry from the add.

for (int i = 0; i < result->size(); i++) {

  x = (x << 8) + (*result)[i];

  (*result)[i] = x >> 1;

  x &= 1;

}

```

#### Line Comments

Also, lines that are non-obvious should get a comment at the end of the line. These end-of-line comments should be separated from the code by 2 spaces. Example:

```C++

// If we have enough memory, mmap the data portion too.

mmap_budget = max<int64>(0, mmap_budget - index_->length());

if (mmap_budget >= data_size_ && !MmapData(mmap_chunk_bytes, mlock))

  return;  // Error already logged.

```

Note that there are both comments that describe what the code is doing, and comments that mention that an error has already been logged when the function returns.

If you have several comments on subsequent lines, it can often be more readable to line them up:

```C++

DoSomething();                  // Comment here so the comments line up.

DoSomethingElseThatIsLonger();  // Two spaces between the code and the comment.

{ // One space before comment when opening a new scope is allowed,

  // thus the comment lines up with the following comments and code.

  DoSomethingElse();  // Two spaces before line comments normally.

}

std::vector<string> list{

                    // Comments in braced lists describe the next element...

                    "First item",

                    // .. and should be aligned appropriately.

                    "Second item"};

DoSomething(); /* For trailing block comments, one space is fine. */

```

#### Function Argument Comments

When the meaning of a function argument is nonobvious, consider one of the following remedies:

*   If the argument is a literal constant, and the same constant is used in multiple function calls in a way that tacitly assumes they're the same, you should use a named constant to make that constraint explicit, and to guarantee that it holds.

*   Consider changing the function signature to replace a `bool` argument with an `enum` argument. This will make the argument values self-describing.

*   For functions that have several configuration options, consider defining a single class or struct to hold all the options , and pass an instance of that. This approach has several advantages. Options are referenced by name at the call site, which clarifies their meaning. It also reduces function argument count, which makes function calls easier to read and write. As an added benefit, you don't have to change call sites when you add another option.

*   Replace large or complex nested expressions with named variables.

*   As a last resort, use comments to clarify argument meanings at the call site.

Consider the following example:

#### badcode// What are these arguments?

const DecimalNumber product = CalculateProduct(values, 7, false, nullptr);

```

versus:

```C++

ProductOptions options;

options.set_precision_decimals(7);

options.set_use_cache(ProductOptions::kDontUseCache);

const DecimalNumber product =

    CalculateProduct(values, options, /*completion_callback=*/nullptr);

```

#### Don'ts

Do not state the obvious. In particular, don't literally describe what code does, unless the behavior is nonobvious to a reader who understands C++ well. Instead, provide higher level comments that describe _why_ the code does what it does, or make the code self describing.

Compare this:

#### badcode// Find the element in the vector.  <-- Bad: obvious!

auto iter = std::find(v.begin(), v.end(), element);

if (iter != v.end()) {

  Process(element);

}

```

To this:

```C++

// Process "element" unless it was already processed.

auto iter = std::find(v.begin(), v.end(), element);

if (iter != v.end()) {

  Process(element);

}

```

Self-describing code doesn't need a comment. The comment from the example above would be obvious:

```C++

if (!IsAlreadyProcessed(element)) {

  Process(element);

}

```

### Punctuation, Spelling and Grammar

Pay attention to punctuation, spelling, and grammar; it is easier to read well-written comments than badly written ones.

Comments should be as readable as narrative text, with proper capitalization and punctuation. In many cases, complete sentences are more readable than sentence fragments. Shorter comments, such as comments at the end of a line of code, can sometimes be less formal, but you should be consistent with your style.

Although it can be frustrating to have a code reviewer point out that you are using a comma when you should be using a semicolon, it is very important that source code maintain a high level of clarity and readability. Proper punctuation, spelling, and grammar help with that goal.

### TODO Comments

Use `TODO` comments for code that is temporary, a short-term solution, or good-enough but not perfect.

`TODO`s should include the string `TODO` in all caps, followed by the name, e-mail address, bug ID, or other identifier of the person or issue with the best context about the problem referenced by the `TODO`. The main purpose is to have a consistent `TODO` that can be searched to find out how to get more details upon request. A `TODO` is not a commitment that the person referenced will fix the problem. Thus when you create a `TODO` with a name, it is almost always your name that is given.

<div>

```C++

// TODO(kl@gmail.com): Use a "*" here for concatenation operator.

// TODO(Zeke) change this to use relations.

// TODO(bug 12345): remove the "Last visitors" feature

```

If your `TODO` is of the form "At a future date do something" make sure that you either include a very specific date ("Fix by November 2005") or a very specific event ("Remove this code when all clients can handle XML responses.").

### Deprecation Comments

Mark deprecated interface points with `DEPRECATED` comments.

You can mark an interface as deprecated by writing a comment containing the word `DEPRECATED` in all caps. The comment goes either before the declaration of the interface or on the same line as the declaration.

After the word `DEPRECATED`, write your name, e-mail address, or other identifier in parentheses.

A deprecation comment must include simple, clear directions for people to fix their callsites. In C++, you can implement a deprecated function as an inline function that calls the new interface point.

Marking an interface point `DEPRECATED` will not magically cause any callsites to change. If you want people to actually stop using the deprecated facility, you will have to fix the callsites yourself or recruit a crew to help you.

New code should not contain calls to deprecated interface points. Use the new interface point instead. If you cannot understand the directions, find the person who created the deprecation and ask them for help using the new interface point.
