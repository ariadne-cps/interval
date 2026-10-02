/***************************************************************************
 *            tutorial_interval.cpp
 *
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "ariadne-interval.hpp"

using namespace Ariadne;


template<class T> void print(const char* label, T const& value) {
    std::cout << label << ": " << value << '\n';
}


void section(const char* title) {
    std::cout << "\n=== " << title << " ===\n";
}


void tutorial_interval() {
    //! [Interval tutorial]

    /*
     * ----------------------------------------------------------------------
     * 1. Exact intervals
     * ----------------------------------------------------------------------
     *
     * Interval<T> stores a lower and upper endpoint whose semantics are
     * determined by T.  The standalone Interval layer provides convenient
     * aliases for the exact numeric types supplied by Ariadne Numeric.
     */

    section("Exact intervals");

    DyadicInterval dyadic(Dyadic(-1),Dyadic(2));
    DecimalInterval decimal(Decimal("-1.25"),Decimal("2.50"));
    RationalInterval rational(Rational(-3,2),Rational(7,4));
    RealInterval real(rational);

    print("Dyadic interval",dyadic);
    print("Decimal interval",decimal);
    print("Rational interval",rational);
    print("Real interval",real);

    /*
     * Endpoints are available explicitly.  The midpoint, radius and width
     * derive useful one-dimensional geometric information from the interval.
     */

    print("Rational lower bound",rational.lower_bound());
    print("Rational upper bound",rational.upper_bound());
    print("Rational midpoint",rational.midpoint());
    print("Rational radius",rational.radius());
    print("Rational width",rational.width());


    /*
     * ----------------------------------------------------------------------
     * 2. Predicates
     * ----------------------------------------------------------------------
     *
     * Predicates preserve the logical semantics of their endpoint types.
     * With exact intervals they yield ordinary decidable Boolean values.
     */

    section("Predicates");

    Rational zero(0);
    RationalInterval inside_interval(Rational(-1,2),Rational(1,2));
    RationalInterval outside_interval(Rational(2),Rational(3));

    print("rational contains 0",contains(rational,zero));
    print("inside interval subset of rational",subset(inside_interval,rational));
    print("rational disjoint from [2,3]",disjoint(rational,outside_interval));


    /*
     * ----------------------------------------------------------------------
     * 3. Hull, intersection and splitting
     * ----------------------------------------------------------------------
     *
     * intersection keeps only the common part of two intervals.
     * hull returns the smallest interval containing both arguments.
     * split divides an interval at a suitable midpoint.
     */

    section("Set-like interval operations");

    RationalInterval left(Rational(-2),Rational(1));
    RationalInterval right(Rational(0),Rational(3));

    auto common=intersection(left,right);
    auto joined=hull(left,right);
    auto halves=split(joined);

    print("left",left);
    print("right",right);
    print("intersection",common);
    print("hull",joined);
    print("lower split",halves.first);
    print("upper split",halves.second);


    /*
     * ----------------------------------------------------------------------
     * 4. Exact floating-point intervals
     * ----------------------------------------------------------------------
     *
     * FloatDPExactInterval has FloatDP endpoints.  The endpoint values are
     * exact binary floating-point numbers at the selected precision.
     */

    section("Exact floating-point intervals");

    DoublePrecision dp;
    FloatDP minus_one(-1,dp);
    FloatDP two(2,dp);
    FloatDPExactInterval exact_float(minus_one,two);

    print("FloatDP exact interval",exact_float);
    print("midpoint",exact_float.midpoint());
    print("radius",exact_float.radius());
    print("width",exact_float.width());


    /*
     * ----------------------------------------------------------------------
     * 5. Validated upper intervals
     * ----------------------------------------------------------------------
     *
     * FloatDPUpperInterval represents an outer enclosure.  Its lower endpoint
     * is a directed lower bound and its upper endpoint a directed upper bound.
     *
     * An exact interval can be converted to an upper interval without losing
     * the enclosure guarantee.
     */

    section("Validated upper intervals");

    FloatDPUpperBound one_upper(1,dp);
    FloatDPUpperBound two_upper(2,dp);

    FloatDPUpperInterval upper(-one_upper,two_upper);
    FloatDPUpperInterval enclosed_exact(exact_float);

    print("Validated interval [-1,2]",upper);
    print("Exact interval as validated enclosure",enclosed_exact);

    /*
     * A comparison involving validated information may itself be validated
     * rather than immediately reducible to an ordinary bool.
     */

    print("validated interval contains 0",contains(upper,FloatDP(0,dp)));
    print("validated interval subset of itself",subset(upper,upper));


    /*
     * ----------------------------------------------------------------------
     * 6. Validated interval arithmetic
     * ----------------------------------------------------------------------
     *
     * Arithmetic on FloatDPUpperInterval is outwardly rounded.  Each result
     * therefore encloses every exact value obtained from operands inside the
     * input intervals.
     */

    section("Validated interval arithmetic");

    FloatDPUpperInterval sum=upper+upper;
    FloatDPUpperInterval product=upper*upper;
    FloatDPUpperInterval square=sqr(upper);

    print("upper + upper",sum);
    print("upper * upper",product);
    print("sqr(upper)",square);


    /*
     * ----------------------------------------------------------------------
     * 7. Elementary functions
     * ----------------------------------------------------------------------
     *
     * The same enclosure semantics are maintained by supported elementary
     * functions.
     */

    section("Elementary functions");

    FloatDPUpperInterval positive(-FloatDPUpperBound(0,dp),FloatDPUpperBound(1,dp));

    print("exp([0,1])",exp(positive));
    print("sin([0,1])",sin(positive));
    print("cos([0,1])",cos(positive));


    /*
     * ----------------------------------------------------------------------
     * 8. Generic interval template
     * ----------------------------------------------------------------------
     *
     * The concrete aliases above are all instances of Interval<T>.  Generic
     * code can therefore express the endpoint semantics directly in the type.
     */

    section("Generic Interval<T>");

    Interval<Dyadic> generic(Dyadic(-3),Dyadic(5));
    print("Interval<Dyadic>",generic);
    print("generic midpoint",generic.midpoint());

    //! [Interval tutorial]
}


int main() {
    tutorial_interval();
    return 0;
}
