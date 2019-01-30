I Am You Language Specification
===============================

In [[computing]], a '''programming language specification''' (or '''standard''' or '''definition''') is a documentation artifact that defines a [[programming language]] so that [[programmer|user]]s and [[programming language implementation|implementors]] can agree on what programs in that language mean. Specifications are typically detailed and formal, and primarily used by implementors, with users referring to them in case of ambiguity; the C++ specification is frequently cited by users, for instance, due to the complexity. Related documentation includes a [[programming language reference]], which is intended expressly for users, and a [[programming language rationale]], which explains why the specification is written as it is; these are typically more informal than a specification.

==Standardization==
Not all major programming languages have specifications, and languages can exist and be popular for decades without a specification. A language may have one or more implementations, whose behavior acts as a ''de facto'' standard, without this behavior being documented in a specification. [[Perl]] (through [[Perl 5]]) is a notable example of a language without a specification, while PHP was only specified in 2014, after being in use for 20 years.<ref>[http://hhvm.com/blog/5723/announcing-a-specification-for-php Announcing a specification for PHP], July 30, 2014, Joel Marcey</ref> A language may be implemented and then specified, or specified and then implemented, or these may develop together, which is usual practice today. This is because implementations and specifications provide checks on each other: writing a specification requires precisely stating the behavior of an implementation, and implementation checks that a specification is possible, practical, and consistent. Writing a specification before an implementation has largely been avoided since [[ALGOL 68]] (1968), due to unexpected difficulties in implementation when implementation is deferred. However, languages are still occasionally implemented and gain popularity without a formal specification: an implementation is essential for use, while a specification is desirable but not essential (informally, "code talks").

{{cquote|ALGOL 68 was the first (and possibly one of the last) major language for which a full formal definition was made before it was implemented.|4=[[Cornelis H. A. Koster|C.H.A. Koster]]|5=<ref name="ashoa68">{{cite web | title = A Shorter History of Algol68 | url = http://npt.cc.rsu.ru/user/wanderer/ODP/ALGOL68.txt | accessdate = September 15, 2006 |archiveurl = https://web.archive.org/web/20060810103448/http://npt.cc.rsu.ru/user/wanderer/ODP/ALGOL68.txt <!-- Bot retrieved archive --> |archivedate = August 10, 2006}}</ref>}}

==Forms==
A programming language specification can take several forms, including the following:

* An explicit definition of the [[programming language syntax|syntax]] and [[programming language semantics|semantics]] of the language. While syntax is commonly specified using a formal grammar, semantic definitions may be written in [[natural language]] (e.g., the approach taken for the [[C (programming language)|C language]]), or a [[formal semantics of programming languages|formal semantics]] (e.g., the [[Standard ML]]<ref>{{cite book
   | last = Milner
   | first = R.
   | authorlink = Robin Milner
 |author2=[[Mads Tofte|M. Tofte]] |author3=[[Robert Harper (computer scientist)|R. Harper]] |author4=D. MacQueen
   | title = The Definition of Standard ML (Revised)
   | publisher = MIT Press
   | year = 1997
   | isbn = 0-262-63181-4}}</ref> and [[Scheme (programming language)|Scheme]]<ref>{{cite web|first=Richard |last=Kelsey |author2=William Clinger |author3=Jonathan Rees|title=Section 7.2  Formal semantics|work=Revised<sup>5</sup> Report on the Algorithmic Language Scheme|url = http://www.schemers.org/Documents/Standards/R5RS/HTML/r5rs-Z-H-10.html#%_sec_7.2|date=February 1998|accessdate=2006-06-09}}</ref> specifications). A notable example is the C language, which gained popularity without a formal specification, instead being described as part of a book, ''[[The C Programming Language]]'' (1978), and only much later being formally standardized in [[ANSI C]] (1989).
* A description of the behavior of a [[compiler]] (sometimes called "translator") for the language (e.g., the [[C++]] language and [[Fortran]]).  The syntax and semantics of the language has to be inferred from this description, which may be written in natural or a formal language.
* A [[reference implementation|''model'' implementation]], sometimes written in the language being specified (e.g., [[Prolog]]). The syntax and semantics of the language are explicit in the behavior of the model implementation.

==Syntax==
{{Expand section|date=February 2018}}
The [[syntax]] of a programming language is usually described using a combination of the following two components:
* a [[regular expression]] describing its [[lexeme (computer science)|lexeme]]s, and
* a [[context-free grammar]] which describes how lexemes may be combined to form a syntactically correct program.

==Semantics==
Formulating a rigorous semantics of a large, complex, practical programming language is a daunting task even for experienced specialists, and the resulting specification can be difficult for anyone but experts to understand. The following are some of the ways in which programming language semantics can be described; all languages use at least one of these description methods, and some languages combine more than one<ref>{{cite book
   | last = Jones
   | first = D.
   | authorlink = Derek M. Jones
   | title = Forms of language specification
   | year = 2008
   | url = http://www.knosof.co.uk/vulnerabilities/langconform.pdf
   |accessdate=2012-06-23}}</ref>
* '''[[Natural language]]''': Description by human natural language.
* '''[[Formal semantics of programming languages|Formal semantics]]''': Description by [[mathematics]].
* '''[[Reference implementation]]s''': Description by [[computer program]]
* '''[[Test suite]]s''': Description by examples of programs and their expected behaviors.  While few language specifications start off in this form, the evolution of some language specifications has been influenced by the semantics of a test suite (e.g. in the past the specification of [[Ada (programming language)|Ada]] has been modified to match the behavior of the [[Ada Conformity Assessment Test Suite]]).

===Natural language===
Most widely used languages are specified using natural language descriptions of their semantics.  This description usually takes the form of a ''reference manual'' for the language.  These manuals can run to hundreds of pages, e.g., the print version of ''The Java Language Specification, 3rd Ed.'' is 596 pages long.

The imprecision of natural language as a vehicle for describing programming language semantics can lead to problems with interpreting the specification. For example, the semantics of [[Java (programming language)|Java]] [[thread (computer programming)|threads]] were specified in English, and it was later discovered that the specification did not provide adequate guidance for implementors.<ref>William Pugh. The Java Memory Model is Fatally Flawed. ''Concurrency: Practice and Experience'' 12(6):445-455, August 2000</ref>

===Formal semantics===
{{main|Formal semantics of programming languages}}
Formal semantics are grounded in mathematics. As a result, they can be more precise and less ambiguous than semantics given in natural language. However, supplemental natural language descriptions of the semantics are often included to aid understanding of the formal definitions. For example, The [[International Organization for Standardization|ISO]] Standard for [[Modula-2]] contains both a formal and a natural language definition on opposing pages.

Programming languages whose semantics are described formally can reap many benefits.  For example:
* Formal semantics enable mathematical proofs of program correctness;
* Formal semantics facilitate the design of [[type system]]s, and proofs about the soundness of those type systems;
* Formal semantics can establish unambiguous and uniform standards for implementations of a language.

Automatic tool support can help to realize some of these benefits.  For example, an [[automated theorem prover]] or theorem checker can increase a programmer's (or language designer's) confidence in the correctness of proofs about programs (or the language itself).  The power and scalability of these tools varies widely: full [[formal verification]] is computationally intensive, rarely scales beyond programs containing a few hundred lines{{Citation needed|date=February 2007}} and may require considerable manual assistance from a programmer; more lightweight tools such as [[model checker]]s require fewer resources and have been used on programs containing tens of thousands of lines; many compilers apply static [[type system|type checks]] to any program they compile.

===Reference implementation===
A [[reference implementation]] is a single implementation of a programming language that is designated as authoritative. The behavior of this implementation is held to define the proper behavior of a program written in the language.  This approach has several attractive properties.  First, it is precise, and requires no human interpretation: disputes as to the meaning of a program can be settled simply by executing the program on the reference implementation (provided that the implementation behaves deterministically for that program).

On the other hand, defining language semantics through a reference implementation also has several potential drawbacks.  Chief among them is that it conflates limitations of the reference implementation with properties of the language.  For example, if the reference implementation has a bug, then that bug must be considered to be an authoritative behavior.  Another drawback is that programs written in this language may rely on quirks in the reference implementation, hindering portability across different implementations.

Nevertheless, several languages have successfully used the reference implementation approach.  For example, the [[Perl]] interpreter is considered to define the authoritative behavior of Perl programs.  In the case of Perl, the [[open-source model]] of software distribution has contributed to the fact that nobody has ever produced another implementation of the language, so the issues involved in using a reference implementation to define the language semantics are moot.

===Test suite===
Defining the semantics of a programming language in terms of a [[test suite]] involves writing a number of example programs in the language, and then describing how those programs ought to behave &mdash; perhaps by writing down their correct outputs.  The programs, plus their outputs, are called the "test suite" of the language.  Any correct language implementation must then produce exactly the correct outputs on the test suite programs.

The chief advantage of this approach to semantic description is that it is easy to determine whether a language implementation passes a test suite.  The user can simply execute all the programs in the test suite, and compare the outputs to the desired outputs.  However, when used by itself, the test suite approach has major drawbacks as well.  For example, users want to run their own programs, which are not part of the test suite; indeed, a language implementation that could ''only'' run the programs in its test suite would be largely useless.  But a test suite does not, by itself, describe how the language implementation should behave on any program not in the test suite; determining that behavior requires some extrapolation on the implementor's part, and different implementors may disagree.  In addition, it is difficult to use a test suite to test behavior that is intended or allowed to be [[Nondeterministic programming|nondeterministic]].

Therefore, in common practice, test suites are used only in combination with one of the other language specification techniques, such as a natural language description or a reference implementation.

==See also==
* [[Programming language reference]]

==External links==
===Language specifications===
A few examples of official or draft language specifications:
*Specifications written primarily in formal mathematics:
**[http://mitpress.mit.edu/books/definition-standard-ml The Definition of Standard ML, revised edition] - a formal definition in an [[operational semantics]] style.
**[http://www.schemers.org/Documents/Standards/ Scheme R5RS] - a formal definition in a [[denotational semantics]] style
*Specifications written primarily in natural language:
**[http://www.masswerk.at/algol60/report.htm Algol 60 report]
**[http://www.adahome.com/rm95/ Ada 95 reference manual]
**[http://java.sun.com/docs/books/jls/ Java language specification]
**[http://www.csci.csusb.edu/dick/c++std/cd2/index.html Draft C++ standard]
*Specifications via test suite:
**[http://rubyspec.org/ Ruby's de facto community-driven specification]

==Notes==
<references/>

[[Category:Programming language topics|Specification]]
[[Category:Programming language documentation]]
