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

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "parsing.h"

template <typename T>
auto appended_vector(std::vector<T> x, T const& y) -> std::vector<T>
{
    x.push_back(y);
    return x;
}

template <Parser P>
Parser auto repeat(P parser)
{
    using T = Parser_value_t<P>;
    using Ts = std::vector<T>;
    return reduce_many(
        Ts{},
        parser,
        [](Ts const& ts, T const& t){ return appended_vector(ts, t); }
    );
}

class Ast;

auto foo(std::string_view input) -> Parsed_t<Ast>;

auto syntax(std::string_view input) -> Parsed_t<Ast>;

auto production(std::string_view input) -> Parsed_t<Ast>;

auto expression(std::string_view input) -> Parsed_t<Ast>;

auto term(std::string_view input) -> Parsed_t<Ast>;

auto factor(std::string_view input) -> Parsed_t<Ast>;

auto literal(std::string_view input) -> Parsed_t<Ast>;

auto identifier(std::string_view input) -> Parsed_t<Ast>;
