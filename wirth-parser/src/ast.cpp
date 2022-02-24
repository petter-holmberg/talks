/*
This is free and unencumbered software released std::string::size_typeo the public domain.

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

#include <optional>
#include <memory>
#include <string>

#include "ast.h"

std::string indent(std::string::size_type in)
{
    return std::string(in, ' ');
}

std::string fmt(Ast const& x, std::string::size_type in)
{
    return x.self_->fmt_(in);
}

std::string fmt(Syntax const& x, std::string::size_type in)
{
    std::string res;
    for (auto const& production : x.productions) {
        res += fmt(production, in) + "\n\n";
    }
    return res;
}

std::string fmt(Production const& x, std::string::size_type)
{
    return
        "auto " + fmt(x.identifier, 0) + "(std::string_view input)\n{\n" +
        indent(4) + "return\n" +
        indent(8) + fmt(x.expression, 8) +
        "(input);\n}";
}

std::string fmt(Expression const& x, std::string::size_type in)
{
    if (x.terms.empty() == 1) {
        return fmt(x.term, in);
    } else {
        std::string ret = "choice(\n";
        ret += indent(in + 4) + fmt(x.term, in + 4);
        for (auto t : x.terms) {
            ret += ",\n" + indent(in + 4) + fmt(t, in + 4);
        }
        ret += '\n' + indent(in) + ")";
        return ret;
    }
}

std::string fmt(Term const& x, std::string::size_type in)
{
    if (x.factors.empty()) {
        return fmt(x.factor, in);
    } else {
        std::string ret = "sequence(\n";
        ret += indent(in + 4) + "[](auto";
        for (auto f : x.factors) {
            ret += ", auto";
        }
        ret += "){ return /* TODO */; },\n";
        ret += indent(in + 4) + fmt(x.factor, in);
        for (auto f : x.factors) {
            ret += ",\n" + indent(in + 4) + fmt(f, in + 4);
        }
        ret += '\n' + indent(in) + ")";
        return ret;
    }
}

std::string fmt(Factor const& x, std::string::size_type in)
{
    switch (x.type) {
        case Factor::Type::id_v:
            return fmt(x.value, in);
        case Factor::Type::li_v:
            return fmt(x.value, in);
        case Factor::Type::gr_v:
            return "(\n" + indent(in + 8) + fmt(x.value, in + 8) + '\n' + indent(in + 4) + ')';
        case Factor::Type::op_v:
            return "maybe(\n" + indent(in + 4) + fmt(x.value, in + 4) + '\n' + indent(in) + ')';
        case Factor::Type::re_v:
            return "repeat(\n" + indent(in + 4) + fmt(x.value, in + 4) + '\n' + indent(in) + ')';
    }
    return "";
}

std::string fmt(Literal const& x, std::string::size_type)
{
    if (x.value.length() == 1) {
        return "symbol('" + x.value + "')";
    } else {
        return "str{\"" + x.value + "\"}";
    }
}

std::string fmt(Identifier const& x, std::string::size_type)
{
    return x.value;
}

std::string fmt(Character const& x, std::string::size_type)
{
    return std::string{x.value};
}

std::string fmt(std::vector<Ast> const& xs, std::string::size_type in)
{
    std::string ret;
    for (auto const& x : xs) ret += fmt(x, in);
    return ret;
}
