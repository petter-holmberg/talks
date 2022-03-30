#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
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
"""

"""
Supplement till Biolits frukostföreläsning "Generisk Programmering", 2022-03-30 av Petter Holmberg
"""

import operator
from collections.abc import Callable
from typing import Any

class Roman:
    """
    Förenklade Romerska tal representerade som strängar, med support för addition
    """
    def __init__(self, value: str):
        self.__value = value

    def __str__(self):
        return self.__value

    def __add__(self, other):
        numerals = ["M", "D", "C", "L", "X", "V", "I"]
        i, j = 0, 0
        result = ""
        for n in numerals:
            while i < len(self.__value) and self.__value[i] == n:
                result += n
                i += 1
            while j < len(other.__value) and other.__value[j] == n:
                result += n
                j += 1

        simplified = {"IIIII": "V", "VV": "X", "XXXXX": "L", "LL": "C", "CCCCC": "D", "DD": "M"}
        for key in simplified:
            result = result.replace(key, simplified[key])

        return Roman(result)

def multiply_slow(n: int, a):
    """
    Långsam multiplikation (O(n))
    pre: n >= 1
    a måste ha en vänster-associativ additionsoperator

    >>> multiply_slow(41, 59)
    2419

    >>> str(multiply_slow(41, Roman('LVIIII')))
    'MMCCCCXVIIII'
    """
    if n == 1: return a
    return a + multiply_slow(n - 1, a)

def half(n: int):
    """
    Halvera n och trunkera (binär bit-shift, lätt att implementera i hårdvara)
    pre: n >= 0

    >>> half(41)
    20

    >>> half(40)
    20
    """
    return n >> 1

def odd(n):
    """
    Är n udda? (kolla minst signifikanta biten, lätt att implementera i hårdvara)
    pre: n >= 0

    >>> odd(41)
    True

    >>> odd(40)
    False
    """
    return bool(n & 1)

def multiply(n: int, a):
    """
    Snabb rekursiv multiplikation (O(log n))
    pre: n >= 1
    a måste ha en associativ additionsoperator

    >>> multiply(41, 59)
    2419

    >>> str(multiply(41, Roman('LVIIII')))
    'MMCCCCXVIIII'
    """
    if n == 1: return a
    result = multiply(half(n), a + a)
    if odd(n): result = result + a
    return result

def power(a, n: int):
    """
    Snabb rekursiv exponentiering (O(log n))
    pre: n >= 1
    a måste ha en associativ multiplikationsoperator

    >>> power(41, 3)
    68921
    """
    if n == 1: return a
    result = power(a * a, half(n))
    if odd(n): result = result * a
    return result

def power_semigroup(a, n: int, bin_op: Callable[[Any, Any], Any]):
    """
    Snabb generisk exponentiering för semigrupper, optimerad variant (O(log n))
    pre: n >= 1
    bin_op måste vara en binär associativ operator för a:s typ

    >>> power_semigroup(41, 59, operator.add)
    2419

    >>> str(power_semigroup(Roman('LVIIII'), 41, operator.add))
    'MMCCCCXVIIII'

    >>> power_semigroup(41, 3, operator.mul)
    68921
    """
    def power_accumulate_semigroup(result, a, n, bin_op):
        while (True):
            if odd(n):
                result = bin_op(result, a)
                if n == 1: return result
            a = bin_op(a, a)
            n = half(n)

    while not odd(n):
        a = bin_op(a, a)
        n = half(n)
    if n == 1: return a
    return power_accumulate_semigroup(a, bin_op(a, a), half(n - 1), bin_op)

def power_monoid(a, n: int, bin_op: Callable[[Any, Any], Any], identity_element):
    """
    Snabb exponentiering för monoider (O(log n))
    pre: n >= 0
    bin_op måste vara en binär associativ operator för a:s typ
    pre: bin_op(a, identity_element) == bin_op(identity_element, a) == a

    >>> power_monoid(41, 59, operator.add, 0)
    2419

    >>> power_monoid(41, 0, operator.add, 0)
    0

    >>> power_monoid(41, 0, operator.mul, 1)
    1
    """
    if (n == 0): return identity_element
    return power_semigroup(a, n, bin_op)

def power_group(a, n: int, bin_op: Callable[[Any, Any], Any], identity_element, inverse: Callable[[Any], Any]):
    """
    Snabb exponentiering för grupper (O(log n))
    bin_op måste vara en binär associativ operator för a:s typ
    pre: bin_op(a, identity_element) == bin_op(identity_element, a) == a
    inverse måste vara en funktion returnerar inversen under bin_op för a:s typ

    >>> power_group(41, 59, operator.add, 0, operator.neg)
    2419

    >>> power_monoid(41, 0, operator.add, 0)
    0

    >>> power_group(41, -59, operator.add, 0, operator.neg)
    -2419
    """
    if (n < 0):
        a = inverse(a)
        n = -n
    return power_monoid(a, n, bin_op, identity_element)

def fibonacci_unusable(n):
    """
    Rekursiv Fibonacci, oanvändbart långsam för stora n (O(2^n))
    >>> [fibonacci_unusable(n) for n in range(10)]
    [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
    """
    if n == 0: return 0
    if n == 1: return 1
    return fibonacci_unusable(n - 2) + fibonacci_unusable(n - 1)

def fibonacci_slow(n):
    """
    Fibonacci som en loop, snabb nog i praktiken (O(n))
    >>> [fibonacci_slow(n) for n in range(10)]
    [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
    """
    if (n == 0): return 0
    a, b = 0, 1
    for i in range(1, n):
        a, b = b, a + b
    return b

def multiply_square_matrices(a: list[list[int]], b: list[list[int]]):
    """
    Multiplikation av kvadratiska matriser, representerade som listor av listor

    >>> multiply_square_matrices([[1, 1], [1, 0]], [[2, 1], [1, 1]])
    [[3, 2], [2, 1]]
    """
    ns = range(len(a))
    result = [[0 for row in ns] for col in ns]
    for i in ns:
        for j in ns:
            for k in ns:
                result[i][j] += a[i][k] * b[k][j]
    return result

def multiply_2_by_2_matrices(a: list[list[int]], b: list[list[int]]):
    """
    Multiplikation av 2x2-matriser, representerade som listor av listor
    Optimering för ett specialfall av multiply_square_matrices (inga loopar)

    >>> multiply_2_by_2_matrices([[1, 1], [1, 0]], [[2, 1], [1, 1]])
    [[3, 2], [2, 1]]
    """
    result_0_0 = a[0][0] * b[0][0] + a[0][1] * b[1][0]
    result_0_1 = a[0][0] * b[0][1] + a[0][1] * b[1][1]
    result_1_0 = a[1][0] * b[0][0] + a[1][1] * b[1][0]
    result_1_1 = a[1][0] * b[0][1] + a[1][1] * b[1][1]
    return[[result_0_0, result_0_1], [result_1_0, result_1_1]]

def multiply_fibonacci_matrices(a: list[list[int]], b: list[list[int]]):
    """
    Multiplikation av Fibonaccimatriser, representerade som listor av listor
    Optimering för ett specialfall av multiply_2_by_2 (övre raden kan räknas ut från den undre raden)

    >>> multiply_fibonacci_matrices([[1, 1], [1, 0]], [[2, 1], [1, 1]])
    [[3, 2], [2, 1]]
    """
    result_1_0 = a[1][0] * b[0][0] + a[1][1] * b[1][0]
    result_1_1 = a[1][0] * b[0][1] + a[1][1] * b[1][1]
    return [
        [result_1_0 + result_1_1, result_1_0],
        [result_1_0,              result_1_1]
    ]

def multiply_fibonacci(a: list[int], b: list[int]):
    """
    Multiplikationsoperator för Fibonaccital, representerade som en lista med den undre raden av en Fibonaccimatris
    Optimering av multiply_fibonacci_matrices (all information som behövs finns i den undre raden)

    >>> multiply_fibonacci([1, 0], [1, 1])
    [2, 1]
    """
    return [a[0] * (b[1] + b[0]) + a[1] * b[0], a[0] * b[0] + a[1] * b[1]]

def fibonacci(n):
    """
    Snabb Fibonacci som en optimerad matrismultiplikation, (O(log n))

    >>> [fibonacci(n) for n in range(10)]
    [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
    """
    if n == 0: return 0
    return power_semigroup([1, 0], n, multiply_fibonacci)[0]

def multiply_square_matrices_semiring(a: list[list[Any]], b: list[list[Any]], add_op: Callable[[Any, Any], Any], zero: Any, mul_op: Callable[[Any, Any], Any]):
    """
    Generisk multiplikation av kvadratiska matriser, representerade som listor av listor
    add_op, zero, mul_op måste utgöra en semiring över a:s typ

    >>> multiply_square_matrices_semiring([[1, 1], [1, 0]], [[2, 1], [1, 1]], operator.add, 0, operator.mul)
    [[3, 2], [2, 1]]
    """
    ns = range(len(a))
    result = [[zero for col in ns] for row in ns]
    for i in ns:
        for j in ns:
            for k in ns:
                result[i][j] = add_op(result[i][j], mul_op(a[i][k], b[k][j]))
    return result

def power_square_matrix_semiring(a: list[list[Any]], n: int, add_op: Callable[[Any, Any], Any], zero: Any, mul_op: Callable[[Any, Any], Any], one: Any):
    """
    Generisk exponentiering av kvadratiska matriser, representerade som listor av listor
    add_op, zero, mul_op, one måste utgöra en semiring över a:s typ

    >>> power_square_matrix_semiring([[1, 1], [1, 0]], 0, operator.add, 0, operator.mul, 1)
    [[1, 0], [0, 1]]

    >>> power_square_matrix_semiring([[1, 1], [1, 0]], 1, operator.add, 0, operator.mul, 1)
    [[1, 1], [1, 0]]

    >>> power_square_matrix_semiring([[1, 1], [1, 0]], 2, operator.add, 0, operator.mul, 1)
    [[2, 1], [1, 1]]

    >>> power_square_matrix_semiring([[1, 1], [1, 0]], 3, operator.add, 0, operator.mul, 1)
    [[3, 2], [2, 1]]
    """
    def multiply(a, b):
        return multiply_square_matrices_semiring(a, b, add_op, zero, mul_op)

    ns = range(len(a))
    identity_matrix = [[one if row == col else zero for col in ns] for row in ns]

    return power_monoid(a, n, multiply, identity_matrix)

def multiply_square_matrices_tropical_semiring(a: list[list[int]], b: list[list[int]]):
    """
    Tropisk (min, +) multiplikation av två kvadratiska matriser, representerade som listor av listor
    """
    return multiply_square_matrices_semiring(a, b, min, float('Inf'), operator.add)

def shortest_paths(graph: list[list[int]]):
    """
    Räkna ut längden på den kortaste vägen mellan alla noder i en riktad viktad graf, representerad i matrisform
    """
    return power_monoid(graph, len(graph) - 1, multiply_square_matrices_tropical_semiring, graph)

graph = [
    [0, 6, float('inf'), 3, float('inf'), float('inf'), float('inf')],
    [float('inf'), 0, float('inf'), float('inf'), 2, 10, float('inf')],
    [7, float('inf'), 0, float('inf'), float('inf'), float('inf'), float('inf')],
    [float('inf'), float('inf'), 5, 0, float('inf'), 4, float('inf')],
    [float('inf'), float('inf'), float('inf'), float('inf'), 0, float('inf'), 3],
    [float('inf'), float('inf'), 6, float('inf'), 7, 0, 8],
    [float('inf'), 9, float('inf'), float('inf'), float('inf'), float('inf'), 0]
]

print("Graf från presentationen:")
for line in graph:
    print(line)
print("Kortaste vägen mellan alla noder i grafen:")
for line in shortest_paths(graph):
    print(line)

if __name__ == "__main__":
    import doctest
    doctest.testmod()
