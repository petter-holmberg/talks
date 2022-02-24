/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#include <memory>

#include "parser.h"
#include "ast.h"

/*
Syntax Notation

The word identifier is used to denote nonterminal symbol, and
literal stands for terminal symbol. For brevity, identifier and
character are not defined in further detail.

syntax = {production}.
production = identifier "=" expression ".".
expression = term {"|" term}.
term = factor {factor}.
factor = identifier | literal
            | "(" expression ")"
            | "[" expression "]"
            | "{" expression "}".
literal = """" character {character} """".

Repetition is denoted by curly brackets, i.e. {a} stands for e | a
| aa | aaa | .... Optionality is expressed by square brack-
ets, i.e. [a] stands  for a | e. Parentheses merely serve for
grouping, e.g.(a | b) c stands for ac | bc. Terminal sym-
bols, i.e. literals, are enclosed in quote marks (and, if a quote
mark appears as a literal itself, it is written twice).
*/

// syntax = {production}.
auto syntax(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto ps){ return Ast{Syntax{ps}}; },
            repeat(production)
        )(input);
}

// production = identifier "=" expression "."
auto production(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto id, auto, auto ex, auto){ return Ast{Production{id, ex}}; },
            token(identifier),
            token(symbol('=')),
            expression,
            symbol('.')
        )(input);
}

// expression = term {"|" term}.
auto expression(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto te, auto ts){ return Ast{Expression{te, ts}}; },
            term,
            repeat(
                sequence(
                    [](auto, auto te){ return te; },
                    symbol('|'),
                    term
                )
            )
        )(input);
}

// term = factor {factor}.
auto term(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto fa, auto fs){ return Ast{Term{fa, fs}}; },
            factor,
            repeat(
                factor
            )
        )(input);
}

// factor = identifier | literal | "(" expression ")" | "[" expression "]" | "{" expression "}".
auto factor(std::string_view input) -> Parsed_t<Ast>
{
    return
        token(
            choice(
                sequence(
                    [](auto id){ return Ast{Factor{Factor::Type::id_v, id}}; },
                    identifier
                ),
                sequence(
                    [](auto li){ return Ast{Factor{Factor::Type::li_v, li}}; },
                    literal
                ),
                sequence(
                    [](auto, auto ex, auto){ return Ast{Factor{Factor::Type::gr_v, ex}}; },
                    symbol('('),
                    expression,
                    symbol(')')
                ),
                sequence(
                    [](auto, auto ex, auto){ return Ast{Factor{Factor::Type::op_v, ex}}; },
                    symbol('['),
                    expression,
                    symbol(']')
                ),
                sequence(
                    [](auto, auto ex, auto){ return Ast{Factor{Factor::Type::re_v, ex}}; },
                    symbol('{'),
                    expression,
                    symbol('}')
                )
            )
        )(input);
}

// literal = """" character {character} """".
auto literal(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto, auto st, auto){ return Ast{Literal{st}}; },
            symbol('"'),
            some(
                choice(
                    chain(str("\"\""), [](auto){ return unit('"'); }),
                    satisfy([](char x){ return x != '"'; })
               )
           ),
           symbol('"')
        )(input);
}

// identifier = (letter | "_") {letter | "_" | digit}.
auto identifier(std::string_view input) -> Parsed_t<Ast>
{
    return
        sequence(
            [](auto ch, auto st){ return Ast{Identifier{std::string(1, ch) + st}}; },
            (
                choice(
                    letter,
                    symbol('_')
                )
            ),
            many(
                choice(
                    letter,
                    symbol('_'),
                    digit
                )
            )
        )(input);
}
