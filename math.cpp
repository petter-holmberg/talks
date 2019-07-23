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

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <numeric>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Concepts

#define Semiregular typename
#define Regular typename
#define TotallyOrdered typename
#define AdditiveSemigroup typename
#define AdditiveMonoid typename
#define AdditiveGroup typename
#define MultiplicativeSemigroup typename
#define MultiplicativeMonoid typename
#define MultiplicativeGroup typename
#define Semiring typename
#define Ring typename
#define IntegralDomain typename
#define Field typename
#define SemigroupOperation typename
#define MonoidOperation typename
#define BinaryInteger typename

////////////////////////////////////////////////////////////////
// Type functions

template <Regular T>
struct arithmetic_tf;
template <Regular T>
using ArithmeticType = typename arithmetic_tf<T>::type;

template <Regular T>
struct difference_tf;
template <Regular T>
using DifferenceType = typename difference_tf<T>::type;

template <Regular T>
struct index_tf;
template <Regular T>
using IndexType = typename index_tf<T>::type;

template <AdditiveSemigroup S>
struct zero_tf { static S const value = S{0}; };
#define Zero(S) zero_tf<S>::value

template <MultiplicativeSemigroup S>
struct one_tf { static S const value = S{1}; };
#define One(S) one_tf<S>::value

////////////////////////////////////////////////////////////////
// Equality

template <Semiregular Derived>
struct Equality {};

template <Semiregular Derived>
constexpr auto
operator!=(Equality<Derived> const& x0, Equality<Derived> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(static_cast<Derived const&>(x0) == static_cast<Derived const&>(x1));
}

////////////////////////////////////////////////////////////////
// Total ordering

template <Regular Derived>
struct TotalOrdering : Equality<Derived> {};

template <Regular Derived>
constexpr auto
operator>=(TotalOrdering<Derived> const& x0, TotalOrdering<Derived> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(static_cast<Derived const&>(x0) < static_cast<Derived const&>(x1));
}

template <Regular Derived>
constexpr auto
operator>(TotalOrdering<Derived> const& x0, TotalOrdering<Derived> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return static_cast<Derived const&>(x1) < static_cast<Derived const&>(x0);
}

template <Regular Derived>
constexpr auto
operator<=(TotalOrdering<Derived> const& x0, TotalOrdering<Derived> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(static_cast<Derived const&>(x1) < static_cast<Derived const&>(x0));
}

template <Regular T>
constexpr auto
operator>=(T const& x0, T const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(x0 < x1);
}

template <Regular T>
constexpr auto
operator>(T const& x0, T const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return x1 < x0;
}

template <Regular T>
constexpr auto
operator<=(T const& x0, T const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(x1 < x0);
}

////////////////////////////////////////////////////////////////
// Arithmetic

template <TotallyOrdered Derived>
struct Arithmetic : TotalOrdering<Derived> {};

template <TotallyOrdered Derived>
struct arithmetic_tf<Arithmetic<Derived>> { using type = ArithmeticType<Derived>; };

template <TotallyOrdered Derived>
constexpr auto
operator==(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return static_cast<Derived const&>(x0) == Derived{x1};
}

template <TotallyOrdered Derived>
constexpr auto
operator==(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return Derived{x0} == static_cast<Derived const&>(x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator!=(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return !(x0 == x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator!=(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return !(x0 == x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator<(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return static_cast<Derived const&>(x0) < Derived{x1};
}

template <TotallyOrdered Derived>
constexpr auto
operator<(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return Derived{x0} < static_cast<Derived const&>(x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator>=(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return !(x0 < x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator>=(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return !(x0 < x1);
}

template <TotallyOrdered Derived>
constexpr auto
operator>(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return x1 < x0;
}

template <TotallyOrdered Derived>
constexpr auto
operator>(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return x1 < x0;
}

template <TotallyOrdered Derived>
constexpr auto
operator<=(Arithmetic<Derived> const& x0, ArithmeticType<Arithmetic<Derived>> const& x1) -> bool
{
    return !(x1 < x0);
}

template <TotallyOrdered Derived>
constexpr auto
operator<=(ArithmeticType<Arithmetic<Derived>> const& x0, Arithmetic<Derived> const& x1) -> bool
{
    return !(x1 < x0);
}

// Addition

template <AdditiveSemigroup S>
constexpr auto
operator+(S const& x0, ArithmeticType<S> const& x1) -> S
{
    return x0 + S{x1};
}

template <AdditiveSemigroup S>
constexpr auto
operator+(ArithmeticType<S> const& x0, S const& x1) -> S
{
    return S{x0} + S{x1};
}

template <AdditiveSemigroup S>
struct Plus
{
    constexpr auto
    operator()(S const& x0, S const& x1) const
    {
        return x0 + x1;
    }
};

template <AdditiveSemigroup S>
struct arithmetic_tf<Plus<S>> { using type = S; };

template <AdditiveMonoid M>
constexpr auto
identity_element(Plus<M>)
{
    return Zero(M);
};

// Negation

template <AdditiveGroup G>
struct Negate
{
    constexpr auto
    operator()(G const& x) const
    {
        return -x;
    }
};

// Subtraction

template <AdditiveGroup G0, AdditiveGroup G1>
constexpr auto
operator-(G0 const& x0, G1 const& x1) -> G0
{
    return x0 + (-x1);
}

template <AdditiveGroup G>
constexpr auto
operator-(G const& x0, ArithmeticType<G> const& x1) -> G
{
    return x0 - G{x1};
}

template <AdditiveGroup G>
constexpr auto
operator-(ArithmeticType<G> const& x0, G const& x1) -> G
{
    return G{x0} - x1;
}

// Multiplication

template <MultiplicativeSemigroup S>
constexpr auto
operator*(S const& x0, ArithmeticType<S> const& x1) -> S
{
    return x0 * S{x1};
}

template <MultiplicativeSemigroup S>
constexpr auto
operator*(ArithmeticType<S> const& x0, S const& x1) -> S
{
    return S{x0} * x1;
}

template <MultiplicativeSemigroup S>
struct Times
{
    constexpr auto
    operator()(S const& x0, S const& x1)
    {
        return x0 * x1;
    }
};

template <MultiplicativeSemigroup S>
struct arithmetic_tf<Times<S>> { using type = S; };

template <MultiplicativeMonoid M>
constexpr auto
identity_element(Times<M>)
{
    return One(M);
};

// Division

template <IntegralDomain I>
constexpr auto
operator/(I const& x0, ArithmeticType<I> const& x1) -> I
{
    return x0 / I{x1};
}

template <IntegralDomain I>
constexpr auto
operator/(ArithmeticType<I> const& x0, I const& x1) -> I
{
    return I{x0} / x1;
}

////////////////////////////////////////////////////////////////
// Natural numbers as strings of dots

struct PaleolithicNatural
{
    std::string n;

    PaleolithicNatural() = default;

    // Constructor for the Stone Age person
    PaleolithicNatural(std::string const& x)
       : n(x)
    {}

    // Constructor for the Renissance person
    PaleolithicNatural(unsigned x)
       : n(x, '.')
    {}
};

template<bool dotCompareValue>
auto const
dotCompare =
    [](char leftDot, char rightDot) -> bool
    {
        // Both dots look the same to me
        return dotCompareValue;
    };

// Helper function for splitting a string of dots into two piles, with support for rest elements
template<typename PairHandler, typename LastElementHandler>
auto
splitDots(PaleolithicNatural const& x, PairHandler&& pairHandler, LastElementHandler&& lastHandler) -> void
{
    // Let's go through each pair of dots in this number
    for(auto it = x.n.begin(); it != x.n.end(); ++ it)
    {
        auto next = it + 1;
        // If we have two more dots, run the pair handler
        if(next != x.n.end())
        {
            pairHandler(*it, *next);
            ++ it;
        }
        // Otherwise we have one last dot
        else
        {
            lastHandler(*it);
        }
    }
}

// Natural number type functions

template <>
struct zero_tf<PaleolithicNatural> { static PaleolithicNatural const value; };
PaleolithicNatural const zero_tf<PaleolithicNatural>::value{""};

template <>
struct one_tf<PaleolithicNatural> { static PaleolithicNatural const value; };
PaleolithicNatural const one_tf<PaleolithicNatural>::value{"."};

template <>
struct arithmetic_tf<PaleolithicNatural> { using type = PaleolithicNatural; };

// Natural number total ordering

auto
operator==(PaleolithicNatural const& x0, PaleolithicNatural const& x1) -> bool
{
    return std::equal(x0.n.begin(), x0.n.end(), x1.n.begin(), x1.n.end(), dotCompare<true>);
}

auto
operator!=(PaleolithicNatural const& x0, PaleolithicNatural const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(x0 == x1);
}

auto
operator<(PaleolithicNatural const& x0, PaleolithicNatural const& x1) -> bool
{
    return std::lexicographical_compare(x0.n.begin(), x0.n.end(), x1.n.begin(), x1.n.end(), dotCompare<false>);
}

// Natural number addition

auto
operator+(PaleolithicNatural const& x0, PaleolithicNatural const& x1) -> PaleolithicNatural
{
    return x0.n + x1.n;
}

// Natural number half and is_odd

auto
half(PaleolithicNatural const& x) -> PaleolithicNatural
{
    PaleolithicNatural halfBuilder;
    splitDots(
        x,
        // Split the dots into two piles, for each pair, add one to our return value
        [&halfBuilder](char firstDot, char secondDot)
        {
            halfBuilder.n += firstDot;
        },
        // Ignore the trailing dot as we're rounding down
        [](char restDot) { }
    );
    return halfBuilder;
}

auto
twice(PaleolithicNatural const& x) -> PaleolithicNatural
{
    return x + x;
}

auto
is_odd(PaleolithicNatural const& x) -> bool
{
    bool oddYet = false;
    splitDots(
        x,
        // Split the dots into two piles, we don't care about complete pairs
        [](char firstDot, char secondDot)
        { },
        // If we end up with a rest dot, x is odd
        [&oddYet](char restDot)
        {
            oddYet = true;
        }
    );
    return oddYet;
}

auto
is_even(PaleolithicNatural const& x) -> bool
{
    return !is_odd(x);
}

auto
half(unsigned x) -> unsigned
{
    return x >> 1;
}

auto
twice(unsigned x) -> unsigned
{
    return x + x;
}

auto
is_odd(unsigned x) -> bool
{
    return x & 1;
}

auto
is_even(unsigned x) -> bool
{
    return !is_odd(x);
}

// Natural number multiplication

template <Regular S, BinaryInteger I, SemigroupOperation Op>
constexpr auto
power_accumulate_semigroup(S ret, S x, I n, Op op) -> S
    //[[expects: n > Zero<I>]]
{
    while (true)
    {
        if (is_odd(n)) {
            ret = op(x, ret);
            if (n == One(I)) return ret;
        }
        x = op(x, x);
        n = half(n);
    }
}

template <Regular R, BinaryInteger I, SemigroupOperation Op>
constexpr auto
power_semigroup(R x, I n, Op op) -> R
    //[[expects: n > Zero<I>]]
{
    while (is_even(n))
    {
        x = op(x, x);
        n = half(n);
    }
    n = half(n);
    if (n == Zero(I)) return x;
    return power_accumulate_semigroup(x, op(x, x), n, op);
}

template <Regular R, BinaryInteger I, MonoidOperation Op>
constexpr auto
power_monoid(R x, I n, Op op) -> R
    //[[expects: n >= Zero<I>]]
{
    if (n == Zero(I)) return identity_element(op);
    return power_semigroup(x, n, op);
}

auto
multiply_egyptian(PaleolithicNatural x0, PaleolithicNatural x1) -> PaleolithicNatural
{
    if (x0 == Zero(PaleolithicNatural)) return Zero(PaleolithicNatural);
    if (x0 == One(PaleolithicNatural)) return x1;
    auto y = multiply_egyptian(half(x0), twice(x1)); // Done if x0 is even
    if (is_odd(x0)) y = y + x1;
    return y;
}

auto
multiply_paleolithic(PaleolithicNatural x0, PaleolithicNatural x1) -> PaleolithicNatural
{
    PaleolithicNatural y;
    for (auto dot : x0.n)
    {
        y = y + x1;
    }
    return y;
}

auto
operator*(PaleolithicNatural const& x0, PaleolithicNatural const& x1) -> PaleolithicNatural
{
    return multiply_egyptian(x0, x1);
    return power_monoid(x0, x1, Plus<PaleolithicNatural>{}); // Breaks in Clang, why???
}

////////////////////////////////////////////////////////////////
// Integers

template <Semiring S>
struct Integer : Arithmetic<S>
{
    S m = Zero(S);
    S n = Zero(S);

    Integer() = default;

    Integer(S const& x)
        : m{x}, n{Zero(S)}
    {}

    Integer(S const& x0, S const& x1)
        : m{x0}, n{x1}
    {}

    Integer(int x)
        : m{x < 0 ? Zero(S) : x}
        , n{x < 0 ? -x : Zero(S)}
    {}
};

// Integer type functions

template <Semiring S>
struct arithmetic_tf<Integer<S>> { using type = S; };

template <Semiring S>
struct difference_tf<Integer<S>> { using type = Integer<S>; };

template <Semiring S>
struct zero_tf<Integer<S>> { static Integer<S> const value; };
template <Semiring S>
Integer<S> const zero_tf<Integer<S>>::value = Integer<S>{};

template <Semiring S>
struct one_tf<Integer<S>> { static Integer<S> const value; };
template <Semiring S>
Integer<S> const one_tf<Integer<S>>::value = Integer<S>{1};

// Integer total ordering

template <Semiring S>
constexpr auto
operator==(Integer<S> const& x0, Integer<S> const& x1) -> bool
{
    return x0.m + x1.n == x1.m + x0.n;
}

template <Semiring S>
constexpr auto
operator!=(Integer<S> const& x0, Integer<S> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(x0 == x1);
}

template <Semiring S>
constexpr auto
operator<(Integer<S> const& x0, Integer<S> const& x1) -> bool
{
    return x0.m + x1.n < x1.m + x0.n;
}

// Integer addition

template <Semiring S>
constexpr auto
operator+(Integer<S> const& x0, Integer<S> const& x1) -> Integer<S>
{
    return {x0.m + x1.m, x0.n + x1.n};
}

// Integer negation

template <Semiring S>
constexpr auto
operator-(Integer<S> const& x) -> Integer<S>
{
    return {x.n, x.m};
}

// Integer multiplication

template <Semiring S>
constexpr auto
operator*(Integer<S> const& x0, Integer<S> const& x1) -> Integer<S>
{
    return {x0.m * x1.m + x0.n * x1.n, x0.m * x1.n + x0.n * x1.m};
}

////////////////////////////////////////////////////////////////
// Rational numbers

template <IntegralDomain I>
struct Rational : Arithmetic<Rational<I>>
{
    I p = Zero(I);
    I q = One(I);

    Rational() = default;

    Rational(I const& x)
        : p{x}, q{One(I)}
    {}

    Rational(I const& x0, I const& x1)
        //[[expects: x1 != Zero<I>]]
        : p{x0}, q{x1}
    {}
};

// Rational type functions

template <IntegralDomain I>
struct arithmetic_tf<Rational<I>> { using type = I; };

template <IntegralDomain I>
struct difference_tf<Rational<I>> { using type = Rational<I>; };

template <IntegralDomain I>
struct zero_tf<Rational<I>> { static Rational<I> const value; };
template <IntegralDomain I>
Rational<I> const zero_tf<Rational<I>>::value = Rational<I>{};

template <IntegralDomain I>
struct one_tf<Rational<I>> { static Rational<I> const value; };
template <IntegralDomain I>
Rational<I> const one_tf<Rational<I>>::value = Rational<I>{One(I), One(I)};

// Rational total ordering

template <IntegralDomain I>
constexpr auto
operator==(Rational<I> const& x0, Rational<I> const& x1) -> bool
{
    return x0.p * x1.q == x1.p * x0.q;
}

template <IntegralDomain I>
constexpr auto
operator<(Rational<I> const& x0, Rational<I> const& x1) -> bool
{
    return x0.p * x1.q < x1.p * x0.q;
}

// Rational addition

template <IntegralDomain I>
constexpr auto
operator+(Rational<I> const& x0, Rational<I> const& x1) -> Rational<I>
{
    return {x0.p * x1.q + x0.q * x1.p, x0.q * x1.q};
}

// Rational negation

template <IntegralDomain I>
constexpr auto
operator-(Rational<I> const& x) -> Rational<I>
{
    return {-x.p, x.q};
}

// Rational multiplication

template <IntegralDomain I>
constexpr auto
operator*(Rational<I> const& x0, Rational<I> const& x1) -> Rational<I>
{
    return {x0.p * x1.p, x0.q * x1.q};
}

template <IntegralDomain I>
constexpr auto
multiplicative_inverse(Rational<I> const& x) -> Rational<I>
    //[[expects: x != Zero<Rational<I>>]]
    //[[ensures ret: ret * x == One<Rational<I>>]] // Reciprocal
{
    return {x.q, x.p};
}

template <IntegralDomain I>
constexpr auto
operator/(Rational<I> const& x0, Rational<I> const& x1) -> Rational<I>
    //[[expects: x1 != Zero<I>]]
{
    return {x0 * multiplicative_inverse(x1)};
}

////////////////////////////////////////////////////////////////
// Geometry

template <IntegralDomain I>
struct Point2D;

template <IntegralDomain I>
struct Line2D;

template <IntegralDomain I>
struct Vector2D;

////////////////////////////////////////////////////////////////
// 2D points

template <IntegralDomain I>
struct Point2D : Equality<Point2D<I>>
{
    Rational<I> x;
    Rational<I> y;

    Point2D() = default;

    Point2D(Rational<I> const& a, Rational<I> const& b)
        : x{a}, y{b}
    {}

    Point2D(Line2D<I> const& l0, Line2D<I> const& l1)
        //[[expects: !are_parallel(l0, l1)]]
        : x{(l0.b * l1.c - l1.b * l0.c) / (l0.a * l1.b - l1.a * l0.b)}
        , y{(l0.c * l1.a - l1.c * l0.a) / (l0.a * l1.b - l1.a * l0.b)}
    {}
};

// Point type functions

template <IntegralDomain I>
struct arithmetic_tf<Point2D<I>> { using type = Rational<I>; };

// Point default total ordering

template <IntegralDomain I>
constexpr auto
operator==(Point2D<I> const& p0, Point2D<I> const& p1) -> bool
{
    return p0.x == p1.x && p0.y == p1.y;
}

namespace std {

    template <IntegralDomain I>
    struct less<Point2D<I>>
    {
        constexpr auto
        operator()(Point2D<I> const& p0, Point2D<I> const& p1) const -> bool
        {
            return p0.x < p1.x || (!(p1.x < p0.x) && (p0.y < p1.y));
        }
    };

}

// Point quadrance

template <IntegralDomain I>
constexpr auto
quadrance(Point2D<I> const& p0, Point2D<I> const& p1) -> Rational<I>
{
    return
        power_semigroup(p1.x - p0.x, ArithmeticType<I>{2}, Times<ArithmeticType<Point2D<I>>>{}) +
        power_semigroup(p1.y - p0.y, ArithmeticType<I>{2}, Times<ArithmeticType<Point2D<I>>>{});
}

////////////////////////////////////////////////////////////////
// 2D lines

template <IntegralDomain I>
struct Line2D : Equality<Line2D<I>>
{
    Rational<I> a = Zero(Rational<I>);
    Rational<I> b = One(Rational<I>);
    Rational<I> c = Zero(Rational<I>);

    constexpr
    Line2D() = default;

    constexpr
    Line2D(Rational<I> const& a_, Rational<I> const& b_, Rational<I> const& c_)
        //[[expects: a != Zero<I> || b != Zero<I>]]
        : a{a_}, b{b_}, c{c_}
    {}

    constexpr
    Line2D(Point2D<I> const& p0, Point2D<I> const& p1)
        //[[expects: p0 != p1]]
        : a{p0.y - p1.y}, b{p1.x - p0.x}, c{p0.x * p1.y - p1.x * p0.y}
    {}
};

// Line type functions

template <IntegralDomain I>
struct arithmetic_tf<Line2D<I>> { using type = Rational<I>; };

// Line default total ordering

template <IntegralDomain I>
constexpr auto
operator==(Line2D<I> const& l0, Line2D<I> const& l1) -> bool
{
    return are_coincident(l0, l1);
}

namespace std {

    template <IntegralDomain I>
    struct less<Line2D<I>>
    {
        constexpr auto
        operator()(Line2D<I> const& l0, Line2D<I> const& l1) const -> bool
        {
            return
                l0.a < l1.a ||
                (!(l1.a < l0.a) && (l0.b < l1.b)) ||
                (!(l1.b < l0.b) && (l0.c < l1.c));
        }
    };

}

// Are two lines parallel?

template <IntegralDomain I>
constexpr auto
are_parallel(Line2D<I> const& l0, Line2D<I> const& l1) -> bool
{
    return l0.a * l1.b == l1.a * l0.b;
}

// Are two lines coincident?

template <IntegralDomain I>
constexpr auto
are_coincident(Line2D<I> const& l0, Line2D<I> const& l1) -> bool
{
    return are_parallel(l0, l1) && l0.b * l1.c == l1.b * l0.c;
}

// Are two lines perpendicular?

template <IntegralDomain I>
constexpr auto
are_perpendicular(Line2D<I> const& l0, Line2D<I> const& l1) -> bool
{
    return l0.a * l1.a + l0.b * l1.b == Zero(I);
}

// Unique point which lies on two non-parallel lines

template <IntegralDomain I>
constexpr auto
meet_of_lines(Line2D<I> const& l0, Line2D<I> const& l1) -> Point2D<I>
    //[[expects: !are_parallel(l0, l1)]]
{
    return {l0, l1};
}

// Unique line which passes through two non-equal points

template <IntegralDomain I>
constexpr auto
join_of_points(Point2D<I> const& p0, Point2D<I> const& p1) -> Line2D<I>
    //[[expects: p0 != p1]]
{
    return {p0, p1};
}

// Does a point lie on a line?

template <IntegralDomain I>
constexpr auto
lies_on(Point2D<I> const& p, Line2D<I> const& l) -> bool
{
    return l.a * p.x + l.b * p.y + l.c == Zero(I);
}

// Do three lines pass through the same point?

template <IntegralDomain I>
constexpr auto
are_concurrent(Line2D<I> const& l0, Line2D<I> const& l1, Line2D<I> const& l2) -> bool
{
    return lies_on(meet_of_lines(l0, l1), l2);
}

// Do three points lie on the same line?

template <IntegralDomain I>
constexpr auto
are_collinear(Point2D<I> const& p0, Point2D<I> const& p1, Point2D<I> const& p2) -> bool
{
    return lies_on(p0, join_of_points(p1, p2));
}

// Spread of two lines

template <IntegralDomain I>
constexpr auto
spread(Line2D<I> const& l0, Line2D<I> const& l1) -> ArithmeticType<Line2D<I>>
    //[[expects: !are_parallel(l0, l1)]]
{
    return
        ((l0.a * l1.b - l1.a * l0.b) * (l0.a * l1.b - l1.a * l0.b)) /
        ((l0.a * l0.a + l0.b * l0.b) * (l1.a * l1.a + l1.b * l1.b));
}

////////////////////////////////////////////////////////////////
// 2D vectors

template <IntegralDomain I>
struct Vector2D : Equality<Vector2D<I>>
{
    Rational<I> x;
    Rational<I> y;

    Vector2D() = default;

    Vector2D(Rational<I> const& x_, Rational<I> const& y_)
        : x{x_}, y{y_}
    {}
};

template <IntegralDomain I>
struct arithmetic_tf<Vector2D<I>> { using type = Rational<I>; };

template <IntegralDomain I>
struct zero_tf<Vector2D<I>> { static Vector2D<I> const value; };
template <IntegralDomain I>
Vector2D<I> const zero_tf<Vector2D<I>>::value = Vector2D<I>{};

// Vector default total ordering

template <IntegralDomain I>
constexpr auto
operator==(Vector2D<I> const& v0, Vector2D<I> const& v1) -> bool
{
    return v0.x == v1.x && v0.y == v1.y;
}

namespace std {

    template <IntegralDomain I>
    struct less<Vector2D<I>>
    {
        constexpr auto
        operator()(Vector2D<I> const& v0, Vector2D<I> const& v1) const -> bool
        {
            return v0.x < v1.x || (!(v1.x < v0.x) && (v0.y < v1.y));
        }
    };

}

// Vector addition

template <IntegralDomain I>
constexpr auto
operator+(Vector2D<I> const& v0, Vector2D<I> const& v1) -> Vector2D<I>
{
    return {v0.x + v1.x, v0.y + v1.y};
}

// Vector negation

template <IntegralDomain I>
constexpr auto
operator-(Vector2D<I> const& v) -> Vector2D<I>
{
    return {-v.x, -v.y};
}

// Vector scalar multiplication

template <IntegralDomain I, Field F>
constexpr auto
operator*(F const& s, Vector2D<I> const& v) -> Vector2D<I>
{
    return {s * v.x, s * v.y};
}

template <IntegralDomain I, Field F>
constexpr auto
operator*(Vector2D<I> const& v, F const& s) -> Vector2D<I>
{
    return {v.x * s, v.y * s};
}

// Vector division

template <IntegralDomain I, Field F>
constexpr auto
operator/(Vector2D<I> const& v, F const& s) -> Vector2D<I>
{
    return {v.x / s, v.y / s};
}

// Vector-point algebra

template <IntegralDomain I>
struct difference_tf<Point2D<I>> { using type = Vector2D<I>; };

template <IntegralDomain I>
constexpr auto
operator+(Point2D<I> const& p, DifferenceType<Point2D<I>> const& v) -> Point2D<I>
{
    return {p.x + v.x, p.y + v.y};
}

template <IntegralDomain I>
constexpr auto
operator+(DifferenceType<Point2D<I>> const& v, Point2D<I> const& p) -> Point2D<I>
{
    return p + v;
}

template <IntegralDomain I>
constexpr auto
operator-(Point2D<I> const& p, Vector2D<I> const& v) -> Point2D<I>
{
    return p + (-v);
}

template <IntegralDomain I>
constexpr auto
operator-(Point2D<I> const& p0, Point2D<I> const& p1) -> DifferenceType<Point2D<I>>
{
    return {p0.x - p1.x, p0.y - p1.y};
}

////////////////////////////////////////////////////////////////
// Polynomial

template <Ring R>
struct Polynomial
{
    std::vector<R> a;

    Polynomial() = default;

    explicit Polynomial(std::initializer_list<R> a_)
        //[[expects: a_.empty() ? true : a_.back() != Zero<R>]]
        : a(a_)
    {}
};

template <Ring R>
struct arithmetic_tf<Polynomial<R>> { using type = R; };

template <Ring R>
struct index_tf<Polynomial<R>> { using type = typename std::vector<R>::difference_type; };

template <Ring R>
struct zero_tf<Polynomial<R>> { static Polynomial<R> const value; };
template <Ring R>
Polynomial<R> const zero_tf<Polynomial<R>>::value = Polynomial<R>{};

template <Ring R>
struct one_tf<Polynomial<R>> { static Polynomial<R> const value; };
template <Ring R>
Polynomial<R> const one_tf<Polynomial<R>>::value = Polynomial<R>{One(R)};

// Polynomial degree

template <Ring R>
constexpr auto
degree(const Polynomial<R>& x) -> IndexType<Polynomial<R>>
{
    return std::rend(x.a) - std::find_if(std::rbegin(x.a), std::rend(x.a), [](R const& x){ return x != Zero(R); }) - 1;
}

// Polynomial total ordering

template <Ring R>
constexpr auto
operator==(Polynomial<R> const& p0, Polynomial<R> const& p1) -> bool
{
    return p0.a == p1.a;
}

template <Ring R>
constexpr auto
operator!=(Polynomial<R> const& x0, Polynomial<R> const& x1) -> bool
    //[[ensures ret: ret != (x0 == x1)]] // Complement
{
    return !(x0 == x1);
}

template <Ring R>
constexpr auto
operator<(Polynomial<R> const& p0, Polynomial<R> const& p1) -> bool
{
    return degree(p0) < degree(p1) || (!(degree(p1) < degree(p0)) && p0.a < p1.a);
}

// Polynomial addition

template <Ring R>
constexpr auto
operator+(Polynomial<R> const& x0, Polynomial<R> const& x1) -> Polynomial<R>
{
    Polynomial<R> y;
    if (x1.a.size() < x0.a.size()) {
        std::transform(std::begin(x1.a), std::end(x1.a), std::begin(x0.a), std::back_inserter(y.a), Plus<R>());
        std::copy(std::begin(x0.a) + x1.a.size(), std::end(x0.a), std::back_inserter(y.a));
    } else {
        std::transform(std::begin(x0.a), std::end(x0.a), std::begin(x1.a), std::back_inserter(y.a), Plus<R>());
        std::copy(std::begin(x1.a) + x0.a.size(), std::end(x1.a), std::back_inserter(y.a));
    }
    return y;
}

// Polynomial negation

template <Ring R>
constexpr auto
operator-(Polynomial<R> x) -> Polynomial<R>
{
    std::transform(std::begin(x.a), std::end(x.a), std::begin(x.a), Negate<R>());
    return x;
}

// Polynomial multiplication

template <Ring R>
constexpr auto
operator*(Polynomial<R> x0, Polynomial<R> x1) -> Polynomial<R>
{
    if (x0 == Zero(Polynomial<R>)) return Zero(Polynomial<R>);
    if (x1 == Zero(Polynomial<R>)) return Zero(Polynomial<R>);

    if (x0 == One(Polynomial<R>)) return x1;
    if (x1 == One(Polynomial<R>)) return x0;

    auto deg_x0 = degree(x0);
    auto deg_x1 = degree(x1);

    Polynomial<R> y;
    y.a.resize(deg_x0 + deg_x1 + 1);
    std::fill(std::begin(y.a), std::end(y.a), Zero(R));

    auto src0 = std::begin(x0.a);
    auto lim0 = src0 + deg_x0 + 1;
    auto dst_begin = std::begin(y.a);
    while (src0 != lim0) {
        auto src1 = std::begin(x1.a);
        auto lim1 = src1 + deg_x1 + 1;
        auto dst = dst_begin;
        while (src1 != lim1) {
            *dst = *dst + *src0 * *src1;
            ++src1;
            ++dst;
        }
        ++src0;
        ++dst_begin;
    }

    return y;
}

// Polynomial evaluation

template <Ring R>
constexpr auto
evaluate(Polynomial<R> const& p, R const& x) -> R
{
    auto k = degree(p);
    auto r = p.a[k];
    while (k > 0) {
        --k;
        r = (r * x) + p.a[k];
    }
    return r;
}

// Binomial coefficients

template <BinaryInteger I>
constexpr auto
choose(I n, I k) -> I
{
    if (n < k) return Zero(I);
    n = n - k;
    if (n < k) std::swap(n, k);
    I result{One(I)};
    I i{Zero(I)};
    while (i < k) {
        ++i;
        ++n;
        result = result * n;
        result = result / i;
    }
    return result;
}

// Subderivatives

template <Ring R>
constexpr auto
subderivative(Polynomial<R> const& p, IndexType<Polynomial<R>> const& k) -> Polynomial<R>
    //[[expects: k < degree(p)]]
{
    if (k == 0) return p;
    Polynomial<R> y;
    y.a.resize(degree(p) + 1 - k);
    auto pos0 = std::begin(p.a) + k;
    auto pos1 = std::begin(y.a);
    auto i = Zero(IndexType<Polynomial<R>>);
    while (pos1 != std::end(y.a)) {
        *pos1 = *pos0 * choose(i + k, k);
        ++pos0;
        ++pos1;
        ++i;
    }
    return y;
}

////////////////////////////////////////////////////////////////
// Tests

template <Regular T>
void assertRegular(T& x)
{
    // Default constructor
    T y;
    T z;

    // Equality
    assert(x == x);

    // Copy assignment
    y = x;
    assert(y == x);

    // Move assignment
    z = std::move(y);
    assert(z == x);

    // Copy constructor
    T w(x);
    assert(w == x);

    // Move constructor
    T v(std::move(w));
    assert(v == x);

    // Default total ordering
    std::less<T> lt;
    assert(!lt(x, x));

    // Destructor
}

template <TotallyOrdered T>
void assertTotallyOrdered(T& x0, T& x1)
    //[[expects: x0 < x1]]
{
    // Regular
    assertRegular(x0);

    // Equality
    assert(x0 != x1);
    assert(!(x0 == x1));

    // Natural total ordering
    assert(!(x0 < x0));
    assert(x0 < x1);
    assert(x1 > x0);
    assert(x0 <= x1);
    assert(x1 >= x0);
    assert(!(x1 < x0));
    assert(!(x0 > x1));
    assert(!(x1 <= x0));
    assert(!(x0 >= x1));
}

template <AdditiveSemigroup S>
void assertAdditiveSemigroup(S const& x0, S const& x1, S const& x2)
{
    // Commutative
    assert(x0 + x1 == x1 + x0);

    // Associative
    assert((x0 + x1) + x2 == x0 + (x1 + x2));
}

template <MultiplicativeSemigroup S>
void assertMultiplicativeSemigroup(S const& x0, S const& x1, S const& x2)
{
    // Associative
    assert((x0 * x1) * x2 == x0 * (x1 * x2));
}

template <AdditiveMonoid M>
void assertAdditiveMonoid(M const& x0, M const& x1, M const& x2)
{
    assertAdditiveSemigroup(x0, x1, x2);

    // Identity
    assert(x0 + Zero(M) == x0);
    assert(x0 + Zero(M) == Zero(M) + x0);
}

template <AdditiveGroup G>
void assertAdditiveGroup(G const& x0, G const& x1, G const& x2)
    //[[expects: x0 != Zero<G> && x1 != Zero<G> && x2 != Zero<G>]]
{
    assertAdditiveMonoid(x0, x1, x2);

    // Cancellation
    assert(x0 + (-x0) == Zero(G));
    assert(x0 + (-x0) == -x0 + x0);
    assert(-(-x0) == x0);

    // Subtraction
    assert(x0 - Zero(G) == x0);
    assert(Zero(G) - x0 == -x0);
    assert(x0 - x1 == x0 + (-x1));
}

template <MultiplicativeMonoid M>
void assertMultiplicativeMonoid(M const& x0, M const& x1, M const& x2)
{
    assertMultiplicativeSemigroup(x0, x1, x2);

    // Identity
    assert(x0 * One(M) == x0);
    assert(x0 * One(M) == One(M) * x0);
}

template <MultiplicativeGroup G>
void assertMultiplicativeGroup(G const& x0, G const& x1, G const& x2)
    //[[expects: x0 != Zero<G> && x1 != Zero<G> && x2 != Zero<G>]]
    //[[expects: x0 != One<G> && x1 != One<G> && x2 != One<G>]]
{
    assertMultiplicativeMonoid(x0, x1, x2);

    // Division
    assert(x0 / One(G) == x0);
    assert(x0 / x0 == One(G));
}

template <Semiring S>
void assertSemiring(S const& x0, S const& x1, S const& x2)
    //[[expects: x0 != Zero<S> && x1 != Zero<S> && x2 != Zero<S>]]
    //[[expects: x0 != One<S> && x1 != One<S> && x2 != One<S>]]
{
    assertAdditiveMonoid(x0, x1, x2);
    assertMultiplicativeMonoid(x0, x1, x2);

    assert(Zero(S) != One(S));

    // Distributive
    assert(x0 * (x1 + x2) == x0 * x1 + x0 * x2);
    assert((x0 + x1) * x2 == x0 * x2 + x1 * x2);
}

template <Ring R>
void assertRing(R const& x0, R const& x1, R const& x2)
    //[[expects: x0 != Zero<R> && x1 != Zero<R> && x2 != Zero<R>]]
    //[[expects: x0 != One<R> && x1 != One<R> && x2 != One<R>]]
{
    assertSemiring(x0, x1, x2);
    assertAdditiveGroup(x0, x1, x2);
}

template <IntegralDomain I>
void assertIntegralDomain(I const& x0, I const& x1, I const& x2)
    //[[expects: x0 != Zero<I> && x1 != Zero<I> && x2 != Zero<I>]]
    //[[expects: x0 != One<I> && x1 != One<I> && x2 != One<I>]]
{
    assertRing(x0, x1, x2);
    assert(x0 * Zero(I) == Zero(I));
    assert(Zero(I) * x0 == Zero(I));
    assert(x0 * One(I) != Zero(I));
    assert(One(I) * x0 != Zero(I));
}

template <Field F>
void assertField(F const& x0, F const& x1, F const& x2)
    //[[expects: x0 != Zero<I> && x1 != Zero<I> && x2 != Zero<I>]]
    //[[expects: x0 != One<I> && x1 != One<I> && x2 != One<I>]]
{
    assertIntegralDomain(x0, x1, x2);
    assertMultiplicativeGroup(x0, x1, x2);
}

int main()
{
    using N = PaleolithicNatural;
    using Z = Integer<N>;
    using Q = Rational<Z>;
    using Point = Point2D<Z>;
    using Line = Line2D<Z>;
    using Vec = Vector2D<Z>;

    // Natural numbers
    {
        N n0{2};
        N n1{3};

        assertTotallyOrdered(n0, n1);

        // Addition
        N n2{5};
        assert(n0 + n1 == n2);
        assertAdditiveSemigroup(n0, n1, n2);

        // Multiplication
        N n3{6};
        assert(n0 * n1 == n3);
        assertSemiring(n0, n1, n2);
    }

    // Integers
    {
        Z z0{Z{-2}};
        Z z1{3};

        assertTotallyOrdered(z0, z1);

        // Addition
        {
            Z z2{1};
            assert(z0 + z1 == z2);
            assertAdditiveGroup(z0, z1, z2);
        }

        // Subtraction
        {
            Z z2{Z{-5}};
            assert(z0 - z1 == z2);
        }

        // Multiplication
        {
            Z z2{Z{-6}};
            assert(z0 * z1 == z2);
            assertIntegralDomain(z0, z1, z2);

            // Commutative
            assert(z0 * z1 == z1 * z0);
        }
    }

    // Rational numbers
    {
        Q q0{Z{-1}, Z{2}};
        Q q1{Z{3}, Z{4}};

        // Addition
        {
            Q q2{Z{1}, Z{4}};
            assert(q0 + q1 == q2);
            assertAdditiveGroup(q0, q1, q2);
        }

        // Subtraction
        {
            Q q2{Z{-5}, Z{4}};
            assert(q0 - q1 == q2);
        }

        // Multiplication
        {
            Q q2{Z{-3}, Z{8}};
            assert(q0 * q1 == q2);
            assertSemiring(q0, q1, q2);

            // Commutative
            assert(q0 * q1 == q1 * q0);
        }

        // Division
        {
            Q q2{Z{-2}, Z{3}};
            assert(q0 / q1 == q2);
            assert(q0 / q1 == q0 * multiplicative_inverse(q1));
            assertField(q0, q1, q2);
        }
    }

    // 2D points
    {
        Point p0{Z{0}, Z{0}};
        Point p1{Z{4}, Z{0}};
        Point p2{Z{4}, Z{3}};

        assertRegular(p0);

        assert(quadrance(p0, p1) == Z{16});
        assert(quadrance(p1, p0) == Z{16});
        assert(quadrance(p1, p2) == Z{9});
        assert(quadrance(p2, p1) == Z{9});
        assert(quadrance(p2, p0) == Z{25});
        assert(quadrance(p0, p2) == Z{25});
    }

    // 2D lines
    {
        Line l0{Z{3}, Z{4}, Z{-1}};

        assertRegular(l0);

        // Parallelism
        {
            Line l0{Z{3}, Z{4}, Z{-1}};
            Line l1{Z{6}, Z{8}, Z{5}};
            Line l2{Z{-4}, Z{3}, Z{2}};
            assert(are_parallel(l0, l0));
            assert(are_parallel(l0, l1));
            assert(are_parallel(l1, l0));
            assert(!are_parallel(l0, l2));
        }

        // Coincidence
        {
            Line l0{Z{3}, Z{4}, Z{-1}};
            Line l1{Z{6}, Z{8}, Z{-2}};
            Line l2{Z{-4}, Z{3}, Z{2}};
            assert(are_coincident(l0, l0));
            assert(are_coincident(l0, l1));
            assert(are_coincident(l1, l0));
            assert(!are_coincident(l0, l2));
        }

        // Perpendicularity
        {
            Line l0{Z{3}, Z{4}, Z{-1}};
            Line l1{Z{6}, Z{8}, Z{5}};
            Line l2{Z{-4}, Z{3}, Z{2}};
            assert(!are_perpendicular(l0, l0));
            assert(!are_perpendicular(l0, l1));
            assert(are_perpendicular(l0, l2));
            assert(are_perpendicular(l2, l0));
        }

        // Meet of lines
        {
            Line l0{Z{5}, Z{12}, Z{-12}};
            Line l1{Z{1}, Z{2}, Z{-3}};
            Point p0{Z{6}, -Q{Z{3}, Z{2}}};

            assert(meet_of_lines(l0, l1) == p0);
        }

        // Join of points
        {
            Point p0{Z{0}, Z{1}};
            Point p1{Z{6}, -Q{Z{3}, Z{2}}};
            Line l0{Z{5}, Z{12}, Z{-12}};

            assert(join_of_points(p0, p1) == l0);
            assert(lies_on(p0, l0));
            assert(lies_on(p1, l0));
        }

        // Coincidence of point and line
        {
            Point p0{Z{6}, -Q{Z{3}, Z{2}}};
            Line l0{Z{1}, Z{2}, Z{-3}};

            assert(lies_on(p0, l0));
        }

        // Concurrency of lines
        {
            Line l0{Z{5}, Z{12}, Z{-12}};
            Line l1{Z{1}, Z{2}, Z{-3}};
            Line l2{Z{0}, Z{2}, Z{3}};

            assert(are_concurrent(l0, l1, l2));
        }

        // Collinearity of points
        {
            Point p0{Z{0}, Z{1}};
            Point p1{Z{6}, -Q{Z{3}, Z{2}}};
            Point p2{Z{3}, -Q{Z{1}, Z{4}}};

            assert(are_collinear(p0, p1, p2));
        }

        // Spread
        {
            Line l0{Z{3}, Z{2}, Z{1}};
            Line l1{Z{1}, Z{-4}, Z{2}};

            Q x{Z{196}, Z{221}};
            assert(spread(l0, l1) == x);
        }
    }

    // 2D vectors
    {
        Vec v0{Z{2}, Z{3}};
        Vec v1{Z{2}, Z{-4}};

        assertRegular(v0);

        // Addition
        {
            Vec v2{Z{4}, Z{-1}};
            assert(v0 + v1 == v2);
            assertAdditiveGroup(v0, v1, v2);
        }

        // Subtraction
        {
            Vec v2{Z{0}, Z{7}};
            assert(v0 - v1 == v2);
        }

        // Scalar multiplication
        {
            Vec v1{Z{4}, Z{6}};
            assert(v0 * Z{2} == v1);

            // Commutative
            assert(v0 * Z{2} == Z{2} * v0);
        }

        // Vector division
        {
            Vec v2{Z{1}, Q{Z{3}, Z{2}}};
            assert(v0 / Z{2} == v2);
        }
    }

    // 2D point/vector affine algebra
    {
        Point p0{Z{2}, Z{3}};
        Point p1{Z{6}, Z{9}};
        DifferenceType<Point> v0{Z{4}, Z{6}};

        // Point/vector addition
        {
            assert(v0 + p0 == p1);

            // Commutative
            assert(v0 + p0 == p0 + v0);
        }

        // Point/vector subtraction
        {
            Vec v1{Z{4}, Z{6}};
            assert(p1 - p0 == v1);
            assert(p0 - p1 == -v1);
            assert(p0 - v0 == p0 + (-v0));
        }
    }

    // Polynomial algebra
    {
        Polynomial<Z> p0;
        Polynomial<Z> p1({Z{1}, Z{2}});
        Polynomial<Z> p2({Z{5}, Z{2}, Z{0}, Z{3}});
        Polynomial<Z> p3({Z{6}, Z{4}, Z{0}, Z{3}, Z{0}, Z{0}});

        assertTotallyOrdered(p0, p1);
        assertTotallyOrdered(p1, p2);
        assertTotallyOrdered(p2, p3);

        // Degree
        {
            assert(degree(p0) < 0);
            assert(degree(p1) == 1);
            assert(degree(p2) == 3);
            assert(degree(p3) == 3);
        }

        // Evaluation
        {
            assert(evaluate(p2, Z{0}) == Z{5});
            assert(evaluate(p2, Z{1}) == Z{10});
            assert(evaluate(p2, Z{2}) == Z{33});
        }

        // Addition
        {
            Polynomial<Z> p3({Z{6}, Z{4}, Z{0}, Z{3}});
            assert(p1 + p2 == p3);
            assertAdditiveGroup(p0, p1, p2);
        }

        // Subtraction
        {
            Polynomial<Z> p3({Z{-4}, Z{0}, Z{0}, Z{-3}});
            assert(p1 - p2 == p3);
        }

        // Multiplication
        {
            Polynomial<Z> p3({Z{5}, Z{12}, Z{4}, Z{3}, Z{6}});
            assert(p1 * p2 == p3);
            assert(p2 * p1 == p3);
            assertMultiplicativeMonoid(p1, p2, p3);
            assertRing(p0, p1, p2);
        }

        // Scalar multiplication
        {
            Polynomial<Z> p3({Z{2}, Z{4}});
            assert(p1 * Z{2} == p3);

            // Commutative
            assert(p1 * Z{2} == Z{2} * p1);
        }

        // Binomial coefficients
        {
            assert(choose(0, 0) == 1);

            assert(choose(1, 0) == 1);
            assert(choose(1, 1) == 1);

            assert(choose(2, 0) == 1);
            assert(choose(2, 1) == 2);
            assert(choose(2, 2) == 1);

            assert(choose(3, 0) == 1);
            assert(choose(3, 1) == 3);
            assert(choose(3, 2) == 3);
            assert(choose(3, 3) == 1);

            assert(choose(4, 0) == 1);
            assert(choose(4, 1) == 4);
            assert(choose(4, 2) == 6);
            assert(choose(4, 3) == 4);
            assert(choose(4, 4) == 1);

            assert(choose(5, 0) == 1);
            assert(choose(5, 1) == 5);
            assert(choose(5, 2) == 10);
            assert(choose(5, 3) == 10);
            assert(choose(5, 4) == 5);
            assert(choose(5, 5) == 1);
        }

        // Subderivatives
        {
            Polynomial<Z> p0({Z{8}, Z{-5}, Z{0}, Z{4}, Z{-1}});
            Polynomial<Z> p1({Z{-5}, Z{0}, Z{12}, Z{-4}});
            Polynomial<Z> p2({Z{0}, Z{12}, Z{-6}});
            Polynomial<Z> p3({Z{4}, Z{-4}});
            Polynomial<Z> p4({Z{-1}});

            assert(subderivative(p0, 0) == p0);
            assert(subderivative(p0, 1) == p1);
            assert(subderivative(p0, 2) == p2);
            assert(subderivative(p0, 3) == p3);
            assert(subderivative(p0, 4) == p4);
        }
    }
}
