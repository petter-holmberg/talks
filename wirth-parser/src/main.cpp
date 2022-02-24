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

#include <iostream>
#include <string>

#include "ast.h"
#include "parser.h"

int main()
{
    std::string test_input{
        "syntax = {production}.\n"
        "production = identifier \"=\" expression \".\".\n"
        "expression = term {\"|\" term}.\n"
        "term = factor {factor}.\n"
        "factor = identifier | literal\n"
        "           | \"(\" expression \")\"\n"
        "           | \"[\" expression \"]\"\n"
        "           | \"{\" expression \"}\".\n"
        "literal = \"\"\"\" character {character} \"\"\"\".\n"
        "identifier = (letter | \"_\") {letter | \"_\" | digit}.\n"
    };

    std::cout << "Input:\n------\n" << test_input << '\n';
    if (auto const& result = syntax(test_input)) {
        std::cout << "Output:\n-------\n" << fmt(result->first, 0);
    } else {
        std::cout << "Error: Could not parse input.\n";
    }
}
