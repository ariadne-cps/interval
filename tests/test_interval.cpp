/***************************************************************************
 *            test_interval.cpp
 *
 *  Copyright  2006-20  Alberto Casagrande, Pieter Collins
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

#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#include "numeric/rational.hpp"
#include "numeric/real.hpp"
#include "interval/interval.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;
using namespace std;

namespace {

constexpr bool check_concept()
{
    using I=ExactIntervalType;
    return requires(DoublePrecision pr, Int n, Nat m, ExactDouble d, FloatDP x, FloatDP a, FloatDP b,
                    ExactIntervalType xivl, UpperIntervalType uivl) {
        uivl=I(); uivl=I(n); uivl=I(m); uivl=I(d); uivl=I(x); uivl=I(xivl);
        uivl=I(n,n); uivl=I(m,m); uivl=I(d,d); uivl=I(a,b);
        uivl=I(n,m); uivl=I(m,d); uivl=I(d,n);
        uivl=n; uivl=m; uivl=x; uivl=xivl;
    };
}

static_assert(check_concept());

template<class F>
Void test_upper_interval_implementation(Ariadne::Testing::Context& context, typename F::PrecisionType pr)
{
    using I=Interval<UpperBound<F>>;
    using A=Interval<Approximation<F>>;

    F one(1,pr), two_value(2,pr), three(3,pr), four(4,pr);
    F half(Dyadic(1,1u),pr), minus_half(Dyadic(-1,1u),pr);

    I x(one,two_value);
    I y(three,four);
    I trig(minus_half,half);
    I negative(F(-4,pr),F(-1,pr));
    I crossing(F(-1,pr),one);

    Bounds<F> bounds(1,2,pr);
    ValidatedNumber generic=bounds.generic();
    UpperIntervalFactory<F> fac(pr);

    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, fac.create(generic));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, cast_singleton(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, make_interval(bounds));

    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, max(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, min(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, abs(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, nul(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, pos(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, neg(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sqr(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, hlf(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, rec(x));

    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, add(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sub(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, mul(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, div(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, fma(x,y,x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, pow(x,Nat(2u)));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, pow(x,Int(-2)));

    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sqrt(negative));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sqrt(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, exp(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, log(negative));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, log(crossing));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, log(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sin(trig));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, cos(trig));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, tan(trig));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, asin(trig));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, acos(trig));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, atan(trig));

    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, mag(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, mig(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, eq(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, lt(x,y));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, cast_exact(x));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, class_name<I>());

    A ax{Approximation<F>(one),Approximation<F>(two_value)};
    A ay{Approximation<F>(three),Approximation<F>(four)};
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, add(ax,ay));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, sub(ax,ay));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, mul(ax,ay));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, mag(ax));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, eq(ax,ay));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, cast_exact(ax));
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, cast_exact_interval(ax));

    Interval<F> exact(one,two_value);
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, exact.lower_bound());
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, exact.upper_bound());
    ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, class_name<Interval<F>>());
}

} // namespace

class TestIntervalType : public Ariadne::Testing::ContextFixture
{
    typedef ExactIntervalType I;
    typedef FloatDP R;
  public:
    using Ariadne::Testing::ContextFixture::ContextFixture;
  public:
    Void test_constructors();
    Void test_input();
    Void test_class();
    Void test_comparison();
    Void test_geometric_predicates();
    Void test_arithmetic();
    Void test_implementation_coverage();
    Void regression_tests();
};


Void
TestIntervalType::test_constructors()
{

    FloatDP zero(0,dp);

    // Construct from pair
    ExactIntervalType ivld1(FloatDP(1.125_x,dp),FloatDP(2.25_x,dp));
    ARIADNE_TESTING_PROVE(ivld1.lower_bound().raw()==1.125_x); ARIADNE_TESTING_PROVE(ivld1.upper_bound().raw()==2.25_x);

    // Default constructor
    ExactIntervalType ivld2;
    if(ivld2.lower_bound()>ivld2.upper_bound()) {
        this->ctx().warning("ExactIntervalType default constructor returns an empty set.");
    } else {
        ARIADNE_TESTING_PROVE((Bool)(ivld2==ExactIntervalType(zero,zero)));
    }

    // Constructor without approximations
    RationalInterval ivld3(Rational(21,8),Rational(17,4));
    ARIADNE_TESTING_INSPECT("ivld3", ivld3);
    ARIADNE_TESTING_EXPECT_EQUAL(ivld3.lower_bound(), Rational(21,8));
    ARIADNE_TESTING_EXPECT_EQUAL(ivld3.upper_bound(), Rational(17,4));

    // Constructor from approximate values
    UpperIntervalType ivld4(2.1_pr,3.2_pr);
    ARIADNE_TESTING_EXPECT_LESS_EQUAL(ivld4.lower_bound().raw(), 2.1_pr);
    ARIADNE_TESTING_EXPECT_GREATER_EQUAL(ivld4.upper_bound().raw(), 3.2_pr);

    // ApproximateTag constructor from a single value
    this->ctx().warning("Cannot construct Interval<UpperFloatDP> from Rational.");
//    UpperIntervalType ivld5(Rational(1,3));
//    ARIADNE_TESTING_EXPECT_LESS(cast_exact(ivld5.lower_bound()), Rational(1,3));
//    ARIADNE_TESTING_EXPECT_GREATER(cast_exact(ivld5.upper_bound()), Rational(1,3));

    // ExactTag constructor from a single value
    ExactIntervalType ivld6(FloatDP(1.25_x,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(ivld6.lower_bound(), 1.25_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ivld6.upper_bound(), 1.25_x);

    // Empty interval
    EmptyInterval empty_interval;
    ExactIntervalType ivld7(empty_interval);
    ARIADNE_TESTING_EXPECT_EQUAL(ivld7.lower_bound().raw(), +inf);
    ARIADNE_TESTING_EXPECT_EQUAL(ivld7.upper_bound().raw(), -inf);
}

Void TestIntervalType::test_class()
{
    // Test lower, upper, midpoint, radius, width

    // Tests for exact operations
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).lower_bound(), -0.25_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).upper_bound(), 0.5_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).midpoint(), 0.125_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).centre(), 0.125_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).radius(), 0.375_x);
    ARIADNE_TESTING_EXPECT_EQUAL(ExactIntervalType(-0.25_x,0.50_x).width(), 0.75_x);

}

Void TestIntervalType::test_input()
{
    ExactIntervalType ivl1,ivl2;
    string input("[1.125,2.25] [0.4,0.6]");
    stringstream iss(input);

    iss >> ivl1;
    ivl2=ExactIntervalType(1.125_x,2.25_x);

    ARIADNE_TESTING_INSPECT("ivl1", ivl1);
    ARIADNE_TESTING_INSPECT("ivl2", ivl2);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(equal, ivl1, ivl2);
    ARIADNE_TESTING_PROVE(ivl1.lower_bound()==ivl2.lower_bound() && ivl1.upper_bound()==ivl2.upper_bound());
    ARIADNE_TESTING_PROVE(equal(ivl1,ivl2));

    iss >> ivl1;
    ivl2=ExactIntervalType(0.39999999999999997_pr,0.60000000000000009_pr);
    ARIADNE_TESTING_INSPECT("ivl1", ivl1);
    ARIADNE_TESTING_INSPECT("ivl2", ivl2);
    if(!equal(ivl1,ivl2)) {
        this->ctx().warning("ExactIntervalType string constructor returns an approximate interval, not an outwardly rounded interval.");
    }

    stringstream parentheses("(1:2)");
    parentheses >> ivl1;
    ARIADNE_TESTING_PROVE(equal(ivl1,ExactIntervalType(1,2)));

    stringstream semicolon("[1;2]");
    semicolon >> ivl1;
    ARIADNE_TESTING_PROVE(equal(ivl1,ExactIntervalType(1,2)));

    stringstream incomplete("[1,");
    ARIADNE_TESTING_EXPECT_THROWS_MESSAGE(std::runtime_error, incomplete >> ivl1, "Assertion `");

    stringstream invalid_left("x1,2]");
    ARIADNE_TESTING_EXPECT_THROWS_MESSAGE(std::runtime_error, invalid_left >> ivl1, "Assertion `");

    stringstream invalid_middle("[1x2]");
    ARIADNE_TESTING_EXPECT_THROWS_MESSAGE(std::runtime_error, invalid_middle >> ivl1, "Assertion `");

    stringstream invalid_right("[1,2x");
    ARIADNE_TESTING_EXPECT_THROWS_MESSAGE(std::runtime_error, invalid_right >> ivl1, "Assertion `");
}

Void TestIntervalType::test_comparison() {
    // FIXME: If using Boost style interval tests, uncomment the line below
    // and comment out the line after
    //ARIADNE_TESTING_PROVE(indeterminate(ivld1==ivld2));
    ExactIntervalType ivl1(1.125_x,2.25_x);
    ExactIntervalType ivl2=ivl1;

    ARIADNE_TESTING_PROVE(ivl1==ivl2);
    ExactIntervalType& ivl1ref=ivl1;
    ivl1ref=ExactIntervalType(5.25_x,7.375_x);
    ARIADNE_TESTING_INSPECT("ivl1ref", ivl1ref);
    ARIADNE_TESTING_PROVE(ivl1ref.lower_bound()==5.25_x);
}

Void TestIntervalType::test_geometric_predicates()
{
    ExactIntervalType empty_interval=EmptyInterval();

    ARIADNE_TESTING_INSPECT("empty_interval", empty_interval);

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(intersect, ExactIntervalType(0.0_x,1.5_x), ExactIntervalType(1.5_x,3));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(intersect, ExactIntervalType(0.0_x,1.5_x), ExactIntervalType(1.625_x,3));

    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(disjoint, ExactIntervalType(0.0_x,1.5_x), ExactIntervalType(1.5_x,3));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(disjoint, ExactIntervalType(0.0_x,1.5_x), ExactIntervalType(1.625_x,3));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(subset, ExactIntervalType(1.0_x,1.5_x), ExactIntervalType(1.0_x,2.0_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(subset, ExactIntervalType(1.5_x,2.0_x), ExactIntervalType(1.0_x,2.0_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(subset, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.0_x,2.0_x));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(superset, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.0_x,1.5_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(superset, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.5_x,2.0_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(superset, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.0_x,2.0_x));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(inside, ExactIntervalType(1.25_x,1.75_x), ExactIntervalType(1.0_x,2.0_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(inside, ExactIntervalType(1.00_x,1.75_x), ExactIntervalType(1.0_x,2.0_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(inside, ExactIntervalType(1.25_x,2.00_x), ExactIntervalType(1.0_x,2.0_x));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(covers, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.25_x,1.75_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(covers, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.00_x,1.75_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(covers, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.25_x,2.00_x));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(overlap, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.75_x,3.0_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(overlap, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(2.00_x,2.75_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(overlap, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(2.25_x,2.75_x));

    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(separated, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(1.75_x,3.0_x));
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(separated, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(2.00_x,2.75_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(separated, ExactIntervalType(1.0_x,2.0_x), ExactIntervalType(2.25_x,2.75_x));

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(disjoint, empty_interval, empty_interval);
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(intersect, empty_interval, empty_interval);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(superset, empty_interval, empty_interval);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(subset, empty_interval, empty_interval);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(separated, empty_interval, empty_interval);
    ARIADNE_TESTING_EXPECT_NOT_BINARY_PREDICATE(overlap, empty_interval, empty_interval);

    if(!inside(empty_interval,empty_interval)) {
        this->ctx().warning("inside(empty_interval,empty_interval) returns false");
    }

    if(!covers(empty_interval,empty_interval)) {
        this->ctx().warning("covers(empty_interval,empty_interval) returns false");
    }

    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(disjoint, ExactIntervalType(-1,2), empty_interval);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(subset, empty_interval, ExactIntervalType(0.25_x,0.75_x));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(covers, ExactIntervalType(0.25_x,0.75_x), empty_interval);
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(inside, empty_interval, ExactIntervalType(0.25_x,0.75_x));
}

Void TestIntervalType::test_arithmetic() {
    UpperIntervalType e=EmptyInterval();
    ARIADNE_TESTING_EXPECT_SAME(sqrt(UpperIntervalType(-4,-1)), e);
    ARIADNE_TESTING_EXPECT_SAME(sqrt(UpperIntervalType(-4,0)), UpperIntervalType(0,0));
    ARIADNE_TESTING_EXPECT_BINARY_PREDICATE(refines, UpperIntervalType(-3,3), sqrt(UpperIntervalType(-4,9)));
    ARIADNE_TESTING_EXPECT_SAME(log(UpperIntervalType(-4,-1)), e);
    ARIADNE_TESTING_EXPECT_SAME(log(UpperIntervalType(-4,0)), e);
    ARIADNE_TESTING_EXPECT_SAME(log(UpperIntervalType(-4,1)), UpperIntervalType(-inf,0));
    ARIADNE_TESTING_EXPECT_SAME(log(UpperIntervalType( 0,1)), UpperIntervalType(-inf,0));
}

Void TestIntervalType::test_implementation_coverage()
{
    test_upper_interval_implementation<FloatDP>(this->ctx(), dp);
    test_upper_interval_implementation<FloatMP>(this->ctx(), MultiplePrecision(128_bits));

    FloatDPUpperInterval singleton(1,1);
    FloatDPUpperInterval nonsingleton(1,2);
    auto widened_singleton=widen_domain(singleton);
    ARIADNE_TESTING_EXPECT_LESS(widened_singleton.lower_bound(), singleton.lower_bound().raw());
    ARIADNE_TESTING_EXPECT_GREATER(widened_singleton.upper_bound(), singleton.upper_bound().raw());
    ARIADNE_TESTING_EXPECT_NO_THROW(widen_domain(nonsingleton));

    FloatDPUpperInterval rounds_down(FloatDP(ExactDouble(-1.3),dp),FloatDP(ExactDouble(2.3),dp));
    auto widened_rounds_down=widen_domain(rounds_down);
    ARIADNE_TESTING_EXPECT_LESS_EQUAL(widened_rounds_down.lower_bound(), rounds_down.lower_bound().raw());
    ARIADNE_TESTING_EXPECT_GREATER_EQUAL(widened_rounds_down.upper_bound(), rounds_down.upper_bound().raw());

    ARIADNE_TESTING_EXPECT_NO_THROW(approximate_domain(singleton));
    ARIADNE_TESTING_EXPECT_NO_THROW(approximate_domain(nonsingleton));

    FloatMPUpperInterval mp_default;
    FloatMPUpperInterval mp_empty{EmptyInterval()};
    FloatMPUpperInterval mp_entire{EntireInterval()};
    auto mp_unit=FloatMPUpperInterval::unit_interval();
    ARIADNE_TESTING_PROVE(mp_default.is_empty());
    ARIADNE_TESTING_PROVE(mp_empty.is_empty());
    ARIADNE_TESTING_EXPECT_EQUAL(mp_unit.lower_bound().raw(), FloatMP(-1,FloatMP::get_default_precision()));
    ARIADNE_TESTING_EXPECT_EQUAL(mp_unit.upper_bound().raw(), FloatMP(+1,FloatMP::get_default_precision()));
    ARIADNE_TESTING_EXPECT_EQUAL(mp_entire.lower_bound().raw(), FloatMP(-inf,FloatMP::get_default_precision()));
    ARIADNE_TESTING_EXPECT_EQUAL(mp_entire.upper_bound().raw(), FloatMP(+inf,FloatMP::get_default_precision()));

    ARIADNE_TESTING_EXPECT_NO_THROW(to_time_bounds(Dyadic(1),Dyadic(2)));
    DyadicInterval dyadic_interval(Dyadic(1),Dyadic(2));
    ARIADNE_TESTING_EXPECT_NO_THROW(to_time_bounds(dyadic_interval));

    RealInterval bounded_real(Real(1),Real(2));
    ARIADNE_TESTING_EXPECT_NO_THROW(is_bounded(bounded_real));

    ARIADNE_TESTING_EXPECT_NO_THROW(class_name<RealInterval>());
}

Void TestIntervalType::regression_tests() {

    // Approximate upper intervals must materialise split points from the
    // contained raw float rather than depending on wrapper layout.
    {
        FloatDPUpperInterval x(FloatDP(1.0_x,dp),FloatDP(3.0_x,dp));
        FloatDP midpoint=x.midpoint();
        ARIADNE_TESTING_EXPECT_EQUAL(midpoint, FloatDP(2.0_x,dp));

        auto parts=split(x);
        ARIADNE_TESTING_EXPECT_EQUAL(parts.first.lower_bound().raw(), FloatDP(1.0_x,dp));
        ARIADNE_TESTING_EXPECT_EQUAL(parts.first.upper_bound().raw(), midpoint);
        ARIADNE_TESTING_EXPECT_EQUAL(parts.second.lower_bound().raw(), midpoint);
        ARIADNE_TESTING_EXPECT_EQUAL(parts.second.upper_bound().raw(), FloatDP(3.0_x,dp));
    }

    // Regression test; fails dramatically on certain types of rounding
    {
        UpperIntervalType x(1.5707963267948966_pr,1.5707963267948968_pr);
        UpperIntervalType cosx=cos(x);
        ARIADNE_TESTING_INSPECT("x", x);
        ARIADNE_TESTING_EXPECT_LESS(cosx.lower_bound().raw(), 0.0_x);
        ARIADNE_TESTING_EXPECT_GREATER(cosx.lower_bound().raw(), -1e-14_pr);
        ARIADNE_TESTING_EXPECT_GREATER(cosx.upper_bound().raw(), 0.0_x);
        ARIADNE_TESTING_EXPECT_LESS(cosx.upper_bound().raw(), +1e-14_pr);
        ARIADNE_TESTING_PROVE(cosx.lower_bound().raw()<cosx.upper_bound().raw());
    }

}


int main(int argc, char** argv) {
    std::cout << std::setprecision(20);
    std::cerr << std::setprecision(20);
    Ariadne::Testing::Suite suite("interval");
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_constructors);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_input);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_class);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_comparison);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_geometric_predicates);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_arithmetic);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, test_implementation_coverage);
    ARIADNE_TESTING_ADD_TEST_TO(suite, TestIntervalType, regression_tests);
    return Ariadne::Testing::run_suite(suite, argc, argv);
}
