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

#include <memory>
#include <optional>
#include <string>
#include <vector>

class Ast
{
    struct Concept
    {
        virtual ~Concept() = default;

        virtual std::string fmt_(std::string::size_type in) const = 0;
    };

    template <typename T>
    struct Model : Concept
    {
        explicit Model(T obj) : data_(std::move(obj)) {}

        std::string fmt_(std::string::size_type in) const override { return fmt(data_, in); }

        T data_;
    };

    std::shared_ptr<Concept const> self_;

public:

    template <typename T>
    explicit Ast(T obj) : self_(std::make_shared<Model<T>>(std::move(obj))) {}

    friend std::string fmt(Ast const& x, std::string::size_type in);
};

struct Syntax
{
    std::vector<Ast> productions;
};

std::string fmt(Syntax const& x, std::string::size_type in);

struct Production
{
    Ast identifier;
    Ast expression;
};

std::string fmt(Production const& x, std::string::size_type in);

struct Expression
{
    Ast term;
    std::vector<Ast> terms;
};

std::string fmt(Expression const& x, std::string::size_type in);

struct Term
{
    Ast factor;
    std::vector<Ast> factors;
};

std::string fmt(Term const& x, std::string::size_type in);

struct Factor
{
    enum class Type { gr_v, op_v, re_v, li_v, id_v };

    Type type;
    Ast value;
};

std::string fmt(Factor const& x, std::string::size_type in);

struct Literal
{
    std::string value;
};

std::string fmt(Literal const& x, std::string::size_type in);

struct Identifier
{
    std::string value;
};

std::string fmt(Identifier const& x, std::string::size_type in);

struct Character
{
    char value;
};

std::string fmt(Character const& x, std::string::size_type in);
