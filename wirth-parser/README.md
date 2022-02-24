# A functional parser generator for Niklaus Wirth's WSN grammar

By Petter Holmberg, 2022

This program implements a parser generator for the Extended Backus-Naur Form
proposed by Niklaus Wirth in an article titled "What can we do about
the unnecessary diversity of notation for syntactic definitions?" in the
November 1997 issue of "Communications of the ACM". This became known as
"Wirth Syntax Notation" and is still actively used today.
(ref: https://dl.acm.org/doi/10.1145/359863.359883)

Ironically, there have been many standardized variations of EBNF grammars
derived from this article, leading to even more unnecessary diversity of
notation for syntactic definitions.

The parser generator takes a grammar as input and returns an object of the type
`Ast`, which represents an abstract sytax tree with type-erased nodes.

The function `fmt` takes an `Ast` and returns a string representing a C++ parser
for the parsed grammar using the same functions that the parser generator is
written in.

The `main` function demonstrates this by parsing a grammar for the syntax of the
grammar itself and prints the resulting code, which is very close to the
parser's own source code.

This is not a full-featured parser generator, but could be used as a basis to
implement one. For example, multiple formatting functions could be added to
generate source code for parsers in different programming languages.

Source files:

`parsing.h` - Library of general-purpose parsers and parser combinators.

`parser.h`, `parser.cpp` - Parsers for the productions in the grammar.

`ast.h`, `ast.cpp` - Types representing each production in the grammar,
with a `fmt` function for each, and a polymorphic type `Ast` to represent an
abstract syntax tree of these types.

`main.cpp` - A demo that parses a self-descriptive grammar and prints the
parser source code generated from it.
