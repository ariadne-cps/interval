#!/usr/bin/python3

##############################################################################
#            tutorial_interval.py
#
#  Copyright  2026  Luca Geretti
##############################################################################

# This file is part of Ariadne.
#
# Ariadne is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Ariadne is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Ariadne. If not, see <https://www.gnu.org/licenses/>.

from pyariadne import *


def show(label, value):
    print(f"{label}: {value}")


def section(title):
    print(f"\n=== {title} ===")


def tutorial_interval():
    # 1. Exact intervals
    section("Exact intervals")

    dyadic = DyadicInterval(Dyadic(-1), Dyadic(2))
    decimal = DecimalInterval(Decimal("-1.25"), Decimal("2.50"))
    rational = RationalInterval(Rational(-3, 2), Rational(7, 4))
    real = RealInterval(rational)

    show("Dyadic interval", dyadic)
    show("Decimal interval", decimal)
    show("Rational interval", rational)
    show("Real interval", real)

    show("Rational lower bound", rational.lower_bound())
    show("Rational upper bound", rational.upper_bound())
    show("Rational midpoint", rational.midpoint())
    show("Rational radius", rational.radius())
    show("Rational width", rational.width())

    # 2. Predicates
    section("Predicates")

    zero = Rational(0)
    inside_interval = RationalInterval(Rational(-1, 2), Rational(1, 2))
    outside_interval = RationalInterval(Rational(2), Rational(3))

    show("rational contains 0", contains(rational, zero))
    show("inside interval subset of rational", subset(inside_interval, rational))
    show("rational disjoint from [2,3]", disjoint(rational, outside_interval))

    # 3. Hull, intersection and splitting
    section("Set-like interval operations")

    left = RationalInterval(Rational(-2), Rational(1))
    right = RationalInterval(Rational(0), Rational(3))

    common = intersection(left, right)
    joined = hull(left, right)
    lower_half, upper_half = split(joined)

    show("left", left)
    show("right", right)
    show("intersection", common)
    show("hull", joined)
    show("lower split", lower_half)
    show("upper split", upper_half)

    # 4. Exact floating-point intervals
    section("Exact floating-point intervals")

    dp = DoublePrecision()
    minus_one = FloatDP(-1, dp)
    two = FloatDP(2, dp)
    exact_float = FloatDPExactInterval(minus_one, two)

    show("FloatDP exact interval", exact_float)
    show("midpoint", exact_float.midpoint())
    show("radius", exact_float.radius())
    show("width", exact_float.width())

    # 5. Validated upper intervals
    section("Validated upper intervals")

    one_upper = FloatDPUpperBound(1, dp)
    two_upper = FloatDPUpperBound(2, dp)

    upper = FloatDPUpperInterval(-one_upper, two_upper)
    enclosed_exact = FloatDPUpperInterval(exact_float)

    show("Validated interval [-1,2]", upper)
    show("Exact interval as validated enclosure", enclosed_exact)
    show("validated interval contains 0", contains(upper, FloatDP(0, dp)))
    show("validated interval subset of itself", subset(upper, upper))

    # 6. Validated interval arithmetic
    section("Validated interval arithmetic")

    sum_interval = upper + upper
    product_interval = upper * upper
    square_interval = sqr(upper)

    show("upper + upper", sum_interval)
    show("upper * upper", product_interval)
    show("sqr(upper)", square_interval)

    # 7. Elementary functions
    section("Elementary functions")

    positive = FloatDPUpperInterval(-FloatDPUpperBound(0, dp), FloatDPUpperBound(1, dp))

    show("exp([0,1])", exp(positive))
    show("sin([0,1])", sin(positive))
    show("cos([0,1])", cos(positive))

    # 8. Generic interval template
    section("Generic Interval[T]")

    generic = Interval[Dyadic](Dyadic(-3), Dyadic(5))
    show("Interval[Dyadic]", generic)
    show("generic midpoint", generic.midpoint())


if __name__ == "__main__":
    tutorial_interval()
