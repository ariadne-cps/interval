/***************************************************************************
 *            interval/interval.hpp
 *
 *  Copyright  2013-20  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Interval.
 *
 *  Ariadne Interval is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Interval is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Interval.  If not, see <https://www.gnu.org/licenses/>.
 */

/** @file interval/interval.hpp
 *  @brief Interval types, arithmetic, bounds, and set operations.
 */
#ifndef ARIADNE_INTERVAL_HPP
#define ARIADNE_INTERVAL_HPP

#include "foundation/logical.hpp"
#include "interval.decl.hpp"
#include "numeric/arithmetic.hpp"
#include "numeric/decimal.hpp"
#include "numeric/dyadic.hpp"
#include "numeric/floats.hpp"
#include "numeric/lower_number.hpp"
#include "numeric/number.hpp"
#include "numeric/upper_number.hpp"
#include "utility/module.hpp"

namespace Ariadne {

enum class SplitPart : char;

enum class SplitPart : char { LOWER,
                              MIDDLE,
                              UPPER };
/** @brief Read the value from the input stream. */
InputStream& operator>>(InputStream& is, SplitPart& s);
/** @brief Write the value to the output stream. */
OutputStream& operator<<(OutputStream& os, const SplitPart& s);

/** @brief Compile-time trait for centre. */
template<class U>
struct CentreTrait {
    typedef decltype(declval<U>() - declval<U>()) Type;
};
/** @brief Compile-time trait for centre. */
template<class F>
struct CentreTrait<UpperBound<F>> {
    typedef Approximation<F> Type;
};
/** @brief Compile-time trait for centre. */
template<class F>
struct CentreTrait<LowerBound<F>> {
    typedef Approximation<F> Type;
};
/** @brief Compile-time trait for centre. */
template<class P>
struct CentreTrait<UpperNumber<P>> {
    typedef ApproximateNumber Type;
};

/** @brief Compile-time trait for midpoint. */
template<class U>
struct MidpointTrait {
    typedef decltype(max(-declval<U>(), declval<U>())) Type;
};
/** @brief Compile-time trait for midpoint. */
template<class F>
struct MidpointTrait<UpperBound<F>> {
    typedef F Type;
};
/** @brief Compile-time trait for midpoint. */
template<class F>
struct MidpointTrait<LowerBound<F>> {
    typedef F Type;
};
/** @brief Compile-time trait for midpoint. */
template<class F>
struct MidpointTrait<Bounds<F>> {
    typedef F Type;
};
/** @brief Compile-time trait for midpoint. */
template<class F>
struct MidpointTrait<Ball<F>> {
    typedef F Type;
};
/** @brief Compile-time trait for midpoint. */
template<class P>
struct MidpointTrait<UpperNumber<P>> {
    typedef ExactNumber Type;
};

/** @brief Compile-time trait for radius. */
template<class U>
struct RadiusTrait {
    using M = typename MidpointTrait<U>::Type;
    typedef decltype(cast_positive(declval<U>() - declval<M>())) Type;
};
/** @brief Compile-time trait for width. */
template<class U>
struct WidthTrait {
    using L = decltype(-declval<U>());
    typedef decltype(cast_positive(declval<U>() - declval<L>())) Type;
};

/** @brief Compile-time trait for characteristics. */
template<class U>
struct CharacteristicsTrait<Interval<U>> {
    typedef CharacteristicsType<U> Type;
};

class UnitInterval;
class EmptyInterval;
class EntireInterval;

//! \related FloatDPUpperInterval \related FloatBounds \brief Allows the over-approximating interval \a ivl to be treated an over-approximation to a single point.
template<class F>
Bounds<F> cast_singleton(const Interval<UpperBound<F>>&);
/** @brief Construct interval from the supplied arguments. */
template<class F>
Interval<UpperBound<F>> make_interval(const Bounds<F>&);

/** @brief Factory constructing compatible interval. */
template<class UB>
class IntervalFactory;

/** @brief Factory constructing compatible interval. */
template<class F>
class IntervalFactory<UpperBound<F>> {
    typedef typename F::PrecisionType PrecisionType;
    PrecisionType _precision;

  public:
    /** @brief Construct a \c IntervalFactory from precision. */
    IntervalFactory(PrecisionType precision) : _precision(precision) {}
    /** @brief Construct a compatible object from the supplied value and characteristics. */
    Interval<UpperBound<F>> create(const ValidatedNumber& y) const;
};

/** @brief Factory constructing compatible interval. */
template<class F>
class IntervalFactory<Approximation<F>> {
    typedef typename F::PrecisionType PrecisionType;
    PrecisionType _precision;

  public:
    /** @brief Construct a \c IntervalFactory from precision. */
    IntervalFactory(PrecisionType precision) : _precision(precision) {}
    /** @brief Construct a compatible object from the supplied value and characteristics. */
    ApproximateInterval<F> create(const ApproximateNumber& y) const {
        return ApproximateInterval<F>(y, y, this->_precision);
    }
};

template<class F>
using UpperIntervalFactory = IntervalFactory<UpperBound<F>>;
template<class F>
using ApproximateIntervalFactory = IntervalFactory<Approximation<F>>;

/** @brief Declare interval arithmetic operations for the supported operand types. */
template<class U>
struct DeclareIntervalArithmeticOperations {};
/** @brief Declare interval arithmetic operations for the supported operand types. */
template<class F>
struct DeclareIntervalArithmeticOperations<UpperBound<F>>
    : DeclareNumericOperations<UpperInterval<F>>,
      DeclareComparisonOperations<UpperInterval<F>, ValidatedKleenean>,
      DefineFieldOperators<UpperInterval<F>>,
      DefineComparisonOperators<UpperInterval<F>, ValidatedKleenean>,
      ProvideConvertedFieldOperations<UpperInterval<F>, Bounds<F>>,
      ProvideConvertedComparisonOperations<UpperInterval<F>, Bounds<F>, UpperInterval<F>, ValidatedKleenean>,
      ProvideConcreteGenericElementaryOperations<UpperInterval<F>, ValidatedNumber>,
      ProvideConcreteGenericComparisonOperations<UpperInterval<F>, ValidatedNumber, ValidatedKleenean> {
    typedef typename F::PrecisionType PrecisionType;
    /** @brief Return an upper bound on the absolute value. */
    friend Error<F> mag(const UpperInterval<F>&);
    /** @brief Compute the product of the first two operands plus the third. */
    friend UpperInterval<F> fma(const UpperInterval<F>& x1, const UpperInterval<F>& x2, UpperInterval<F> y);
    /** @brief Return a factory for compatible objects. */
    friend UpperIntervalFactory<F> factory(const UpperInterval<F>& ivl) {
        return UpperIntervalFactory<F>(ivl.upper_bound().precision());
    }

    /** @brief Return the working precision. */
    PrecisionType precision() const;

    /** @brief Return the sum or unary positive value. */
    friend UpperInterval<F> operator+(const UpperInterval<F>& ivl1, const F& x2) {
        return make_interval(cast_singleton(ivl1) + x2);
    }
    /** @brief Return the sum or unary positive value. */
    friend UpperInterval<F> operator+(const F& x1, const UpperInterval<F>& ivl2) {
        return make_interval(x1 + cast_singleton(ivl2));
    }

    /** @brief Return the sum or unary positive value. */
    friend ApproximateInterval<F> operator+(const UpperInterval<F>& ivl1, const Approximation<F>& x2) {
        return ApproximateInterval<F>(ivl1) + x2;
    }
    /** @brief Return the sum or unary positive value. */
    friend ApproximateInterval<F> operator+(const Approximation<F>& x1, const UpperInterval<F>& ivl2) {
        return x1 + ApproximateInterval<F>(ivl2);
    }

    /** @brief Return the product. */
    friend UpperInterval<F> operator*(const UpperInterval<F>& ivl1, const F& x2) {
        return make_interval(cast_singleton(ivl1) * x2);
    }
    /** @brief Return the product. */
    friend UpperInterval<F> operator*(const F& x1, const UpperInterval<F>& ivl2) {
        return make_interval(x1 * cast_singleton(ivl2));
    }

    /** @brief Return the product. */
    friend ApproximateInterval<F> operator*(const UpperInterval<F>& ivl1, const Approximation<F>& x2) {
        return ApproximateInterval<F>(ivl1) * x2;
    }
    /** @brief Return the product. */
    friend ApproximateInterval<F> operator*(const Approximation<F>& x1, const UpperInterval<F>& ivl2) {
        return x1 * ApproximateInterval<F>(ivl2);
    }
};

/** @brief Declare interval arithmetic operations for the supported operand types. */
template<class F>
struct DeclareIntervalArithmeticOperations<Approximation<F>>
    : DeclareNumericOperations<ApproximateInterval<F>>,
      DeclareComparisonOperations<ApproximateInterval<F>, ApproximateKleenean>,
      DefineFieldOperators<ApproximateInterval<F>>,
      DefineComparisonOperators<ApproximateInterval<F>, ApproximateKleenean>,
      ProvideConvertedFieldOperations<ApproximateInterval<F>, Approximation<F>>,
      ProvideConvertedComparisonOperations<ApproximateInterval<F>, Approximation<F>, ApproximateInterval<F>, ApproximateKleenean>,
      ProvideConcreteGenericElementaryOperations<ApproximateInterval<F>, ApproximateNumber>,
      ProvideConcreteGenericComparisonOperations<ApproximateInterval<F>, ApproximateNumber, ApproximateKleenean> {
    typedef typename F::PrecisionType PR;
    typedef PR PrecisionType;
    /** @brief Return a factory for compatible objects. */
    friend ApproximateIntervalFactory<F> factory(const ApproximateInterval<F>& ivl) {
        return ApproximateIntervalFactory<F>(ivl.upper_bound().precision());
    }
    /** @brief Return an upper bound on the absolute value. */
    friend PositiveApproximation<F> mag(const ApproximateInterval<F>& ivl) {
        return cast_positive(max(-ivl.lower_bound(), ivl.upper_bound()));
    }
    /** @brief Return the sum of the operands. */
    friend ApproximateInterval<F> add(const ApproximateInterval<F>& ivl1, const ApproximateInterval<F>& ivl2) {
        return ApproximateInterval<F>(ivl1.lower_bound() + ivl2.lower_bound(), ivl1.upper_bound() + ivl2.upper_bound());
    }
    /** @brief Return the difference of the operands. */
    friend ApproximateInterval<F> sub(const ApproximateInterval<F>& ivl1, const ApproximateInterval<F>& ivl2) {
        return ApproximateInterval<F>(ivl1.lower_bound() - ivl2.upper_bound(), ivl1.upper_bound() - ivl2.lower_bound());
    }
    /** @brief Return the product of the operands. */
    friend ApproximateInterval<F> mul(const ApproximateInterval<F>& ivl1, const ApproximateInterval<F>& ivl2) {
        auto rll = ivl1.lower_bound() * ivl2.lower_bound();
        auto rlu = ivl1.lower_bound() * ivl2.upper_bound();
        auto rul = ivl1.upper_bound() * ivl2.lower_bound();
        auto ruu = ivl1.upper_bound() * ivl2.upper_bound();
        return ApproximateInterval<F>(min(min(rll, rlu), min(rul, ruu)), max(max(rll, rlu), max(rul, ruu)));
    }
    /** @brief Compute the product of the first two operands plus the third. */
    friend ApproximateInterval<F> fma(const ApproximateInterval<F>& x1, const ApproximateInterval<F>& x2, ApproximateInterval<F> y);

    /** @brief Return the working precision. */
    PR precision() const;
};

/** @brief Declare interval arithmetic operations for the supported operand types. */
template<ARawFloat F>
struct DeclareIntervalArithmeticOperations<F> : DeclareIntervalArithmeticOperations<UpperBound<F>> {};

template<class T, class U>
concept ConstructibleGivenDefaultPrecision = requires(const T& t, const U& u) { T(u, t.precision()); };

//! \ingroup GeometryModule
//! \brief Intervals with upper endoint of type \a U.
//! \details
//! Not intended for use in basic interval arithmetic; represents a \em geometric rather than a \em numerical object.
/** @brief Represents an interval of numeric values with endpoints of type U.
 *  @tparam U Endpoint representation determining arithmetic and precision semantics.
 */
template<class U>
class Interval
    : public DeclareIntervalArithmeticOperations<U> {
    typedef typename U::Paradigm P;
    typedef decltype(-declval<U>()) L;
    typedef typename CentreTrait<U>::Type C;
    typedef typename MidpointTrait<U>::Type M;
    typedef typename RadiusTrait<U>::Type R;
    typedef typename WidthTrait<U>::Type W;

  public:
    //! \brief The type returned by the dimension() method.
    typedef SizeOne DimensionType;
    //! \brief The computational paradigm used by the interval.
    typedef P Paradigm;
    //! \brief The type of the lower bound of the interval.
    typedef L LowerBoundType;
    //! \brief The type of the upper bound of the interval.
    typedef U UpperBoundType;
    //! \brief The type returned by the centre() method.
    //! The result must contain the exact centre (mid-point) of the interval.
    typedef C CentreType;
    //! \brief The type returned by the midpoint() method.
    //! The result need not be the exact centre (mid-point) of the interval,
    //! but must be convertible to both the upper and lower bound types.
    typedef M MidpointType;
    //! \brief The type returned by the radius() methods.
    typedef R RadiusType;
    //! \brief The type returned by the width() methods.
    typedef W WidthType;

  public:
    // Deprecated
    typedef Interval<U> NumericType;

  public:
    //! \brief Create the interval which is the single point 0.
    Interval<U> create_zero() const;

    //! \brief Construct an empty interval.
    explicit Interval();
    //! \brief Construct from an empty interval.
    Interval(const EmptyInterval&);
    //! \brief Construct from a unit interval.
    Interval(const UnitInterval&);
    //! \brief Construct from the entire real line.
    Interval(const EntireInterval&);
    //! \brief Construct an interval with the given lower and upper bounds.
    Interval(LowerBoundType l, UpperBoundType u);

    //! \brief Construct an empty interval.
    static Interval<U> empty_interval();
    //! \brief Construct a unit interval.
    static Interval<U> unit_interval();
    //! \brief Construct a biinfinite interval.
    static Interval<U> biinfinite_interval();
    //! \brief Construct a zero interval.
    static Interval<U> singleton_interval(MidpointType x);

    //! \brief Construct a singleton interval from a number.
    template<class V>
        requires Constructible<L, V> and Constructible<U, V>
    explicit Interval(const V& v) : _l(v), _u(v) {}
    /** @brief Construct a \c Interval from v. */
    template<class V>
        requires ConstructibleGivenDefaultPrecision<U, V> and (not Constructible<U, V>)
    explicit Interval(const V& v) : Interval(L(v, RawFloatType<PrecisionType<U>>::get_default_precision()), U(v, RawFloatType<PrecisionType<U>>::get_default_precision())) {}
    //! \brief Assign a singleton interval from a number.
    template<class V>
        requires Assignable<L, V> and Assignable<U, V>
    Interval<U>& operator=(const V& v) {
        _l = v;
        _u = v;
        return *this;
    }
    //! \brief Construct a singleton interval from a number.
    template<class Y, class PR>
        requires Constructible<L, Y, PR> and Constructible<U, Y, PR>
    explicit Interval(const Y& y, PR pr) : _l(y, pr), _u(y, pr) {}

    //! \brief Convert from an interval of a different type.
    template<class UU>
        requires Convertible<UU, U>
    Interval(const Interval<UU>& x) : _l(x.lower_bound()), _u(x.upper_bound()) {}
    //! \brief Construct from an interval of a different type.
    template<class UU>
        requires Constructible<U, UU> and (not Convertible<UU, U>)
    explicit Interval(const Interval<UU>& x) : _l(x.lower_bound()), _u(x.upper_bound()) {}
    //! \brief Construct from an interval of a different type using the given precision.
    template<class UU, class PR>
        requires Constructible<U, UU, PR>
    explicit Interval(const Interval<UU>& x, PR pr) : _l(x.lower_bound(), pr), _u(x.upper_bound(), pr) {}
    //! \brief Construct from an interval of a different type using a default precision.
    template<class UU>
        requires ConstructibleGivenDefaultPrecision<U, UU> and (not Constructible<U, UU>)
    explicit Interval(const Interval<UU>& x) : Interval(x, RawFloatType<PrecisionType<U>>::get_default_precision()) {}
    //! \brief Construct from an interval of a different type using a default precision.
    template<class LL, class UU>
        requires ConstructibleGivenDefaultPrecision<L, LL> and
                 ConstructibleGivenDefaultPrecision<U, UU> and
                 (not(Constructible<L, LL> and Constructible<U, UU>))
    Interval(LL const& l, UU const& u) : Interval(L(l, RawFloatType<PrecisionType<U>>::get_default_precision()), U(u, RawFloatType<PrecisionType<U>>::get_default_precision())) {}

    //! \brief Construct an interval with the lower and upper bounds.
    //! FIXME: Should be explicit, but this would clash with Box constructor from initializer list of double/FloatDP.
    template<class LL, class UU>
        requires Constructible<L, LL> and Constructible<U, UU> and (not(Convertible<LL, L> and Convertible<UU, U>)) and (not(Constructible<L, LL, UU> and Constructible<U, LL, UU>)) // Disambiguate Interval(Y,PR)
    Interval(const LL& l, const UU& u) : _l(l), _u(u) {}

    //! \brief Construct an interval with the lower and upper bounds using the given precision.
    template<class LL, class UU, class PR>
        requires Constructible<L, LL, PR> and Constructible<U, UU, PR>
    Interval(const LL& l, const UU& u, PR pr) : _l(l, pr), _u(u, pr) {}

    //! \brief Assign from an interval of a different type.
    template<class UU>
        requires Assignable<U, UU>
    Interval<U>& operator=(const Interval<UU>& x) {
        _l = x._l;
        _u = x._u;
        return *this;
    }

  public:
    //! \brief The dimension of the set; statically returns size one.
    SizeOne dimension() const;

    //! \brief The lower bound of the interval.
    const LowerBoundType& lower_bound() const;
    //! \brief The upper bound of the interval.
    const UpperBoundType& upper_bound() const;
    //! \brief The centre of the interval, computed with whatever rounding is appropriate.
    CentreType centre() const;
    //! \brief The midpoint of the interval. Need not be the exact midpoint. \sa radius()
    MidpointType midpoint() const;
    //! \brief The radius of the interval. In validated computation, must be an upper bound for the both \c u-m and \c m-l, where \c m is the midpoint. \sa midpoint()
    RadiusType radius() const;
    //! \brief The width of the interval.
    WidthType width() const;

    /** @brief Set the lower bound. */
    void set_lower_bound(LowerBoundType l);
    /** @brief Set the upper bound. */
    void set_upper_bound(UpperBoundType u);
    /** @brief Set the bounds. */
    void set_bounds(LowerBoundType l, UpperBoundType u);

    //! Test if the interval is empty.
    auto is_empty() const -> decltype(declval<L>() > declval<U>());
    //! Test if the interval is a singleton.
    auto is_singleton() const -> decltype(declval<L>() == declval<U>());
    //! Test if the interval is bounded.
    auto is_bounded() const -> decltype(declval<U>() < declval<L>());

  public:
#ifdef DOXYGEN
    //!@{ \name Interval arithmeric
    //!   \brief Only defined for intervals \c U=UpperBound<F> or \c U=Approximation<F> .
    friend Interval<U> operator*(Interval<U> ivl);                         //!< Unary operators \a +,-
    friend Interval<U> operator*(Interval<U> ivl1, Interval<U> ivl2);      //!< Binary arithmetic operators \a +,-,*,/
    friend Interval<U>& operator*=(Interval<U>& ivl1, Interval<U> ivl2);   //!< Inplace binary arithmetic operators \a +=,-=,*=,/=
    friend decltype(auto) operator<=>(Interval<U> ivl1, Interval<U> ivl2); //!< Rich comparison operators \a ==,!=,<=,>=,<,>
    //!@}
#endif

  public:
    L _l;
    U _u;
};

template<class F>
auto DeclareIntervalArithmeticOperations<UpperBound<F>>::precision() const -> PrecisionType {
    const Interval<UpperBound<F>>& ivl = static_cast<const Interval<UpperBound<F>>&>(*this);
    return min(ivl.lower_bound().precision(), ivl.upper_bound().precision());
}

template<class UB>
inline decltype(auto) characteristics(const Interval<UB>& x);

//! \related Interval \brief Write to an output stream.
template<class U>
OutputStream& operator<<(OutputStream& os, const Interval<U>& ivl);

/** @brief Construct a \c Interval with the specified characteristics. */
template<class L, class U>
Interval(L, U) -> Interval<decltype(min(-declval<L>(), declval<U>()))>;

template<class U>
inline auto lower_bound(const Interval<U>& ivl) -> decltype(ivl.lower_bound());
template<class U>
inline auto upper_bound(const Interval<U>& ivl) -> decltype(ivl.upper_bound());
template<class U>
inline auto centre(const Interval<U>& ivl) -> decltype(ivl.centre());
template<class U>
inline auto midpoint(const Interval<U>& ivl) -> decltype(ivl.midpoint());
template<class U>
inline auto radius(const Interval<U>& ivl) -> decltype(ivl.radius());
template<class U>
inline auto width(const Interval<U>& ivl) -> decltype(ivl.width());

//! \related Interval \brief Make an interval with the given lower and upper bounds.
template<class L, class U>
inline Interval<U> make_interval(L l, U u) { return Interval<U>(l, u); }

template<class U>
inline auto is_empty(const Interval<U>& ivl) -> decltype(ivl.lower_bound() > ivl.upper_bound());
//! \related Interval \brief Test if the interval is empty.
template<class U>
inline auto is_empty(const Interval<U>& ivl) -> decltype(ivl.lower_bound() > ivl.upper_bound());
//! \related Interval \brief Test if the interval is a singleton.
template<class U>
inline auto is_singleton(const Interval<U>& ivl) -> decltype(ivl.lower_bound() == ivl.upper_bound());
//! \related Interval \brief Test if the interval is bounded.
template<class U>
inline auto is_bounded(const Interval<U>& ivl) -> decltype(ivl.upper_bound() < ivl.lower_bound());

//! \related Interval \brief Test if \a x1 is an element of the interval \a ivl2.
template<class U, class X>
inline auto element(const X& x1, const Interval<U>& ivl2) -> decltype(ivl2.lower_bound() <= x1 && ivl2.upper_bound() >= x1);
//! \related Interval \brief Test if the interval \a ivl1 contains \a x2.
template<class U, class X>
inline auto contains(const Interval<U>& ivl1, const X& x2) -> decltype(ivl1.lower_bound() <= x2 && ivl1.upper_bound() >= x2);
//! \related Interval \brief Test if the interval \a ivl1 is equal to \a ivl2.
template<class U>
inline auto equal(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() == ivl2.upper_bound());
//! \related Interval \brief Test if the interval \a ivl1 is a subset of \a ivl2.
template<class U1, class U2>
inline auto subset(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() <= ivl2.upper_bound());
//! \related Interval \brief Test if the interval \a ivl1 is a superset of \a ivl2.
template<class U1, class U2>
inline auto superset(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() >= ivl2.upper_bound());
//! \related Interval \brief Test if the interval \a ivl1 is disjoint from \a ivl2. Returns \c false even if the two intervals only have an endpoint in common.
template<class U1, class U2>
inline auto disjoint(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.lower_bound());
//! \related Interval \brief Test if the interval \a ivl1 intersects \a ivl2. Returns \c true even if the two intervals only have an endpoint in common.
template<class U1, class U2>
inline auto intersect(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() >= ivl2.lower_bound());

//! \related Interval \brief Test if the closed interval \a ivl1 is disjoint from the closed interval \a ivl2.
//! Returns \c false if the two intervals only have an endpoint in common.
template<class U1, class U2>
inline auto separated(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.lower_bound());
//! \related Interval \brief Test if the interval \a ivl1 overlaps \a ivl2.
//! Returns \c false if the two intervals only have an endpoint in common.
//! Returns \c true if one of the intervals is a singleton in the interior of the other.
template<class U1, class U2>
inline auto overlap(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() > ivl2.lower_bound());
//! \related Interval \brief Test if the (closed) interval \a ivl1 is a subset of the interior of \a ivl2.
template<class U1, class U2>
inline auto inside(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.upper_bound());
//! \related Interval \brief Test if the interior of the interval \a ivl1 is a superset of the (closed) interval \a ivl2.
template<class U1, class U2>
inline auto covers(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> decltype(ivl1.upper_bound() > ivl2.upper_bound());

//! \related Interval \brief The intersection of two intervals.
template<class U1, class U2>
inline auto intersection(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> Interval<decltype(min(declval<U1>(), declval<U2>()))>;

//! \related Interval \brief The hull of two intervals, equal to the smallest interval containing both as subsets.
template<class U1, class U2>
inline auto hull(const Interval<U1>& ivl1, const Interval<U2>& ivl2) -> Interval<decltype(max(declval<U1>(), declval<U2>()))>;

//! \related Interval \brief The hull of an interval and a point, equal to the smallest interval containing both.
template<class U, class X>
inline auto hull(const Interval<U>& ivl1, X x2) -> Interval<decltype(max(declval<U>(), declval<X>()))>;

//! \related Interval \brief The hull of a point and an interval, equal to the smallest interval containing both.
template<class U, class X>
inline auto hull(X x1, const Interval<U>& ivl2) -> Interval<decltype(max(declval<U>(), declval<X>()))>;

//! \related Interval \brief Split an interval into its lower, middle or upper half.
template<class U>
inline auto split(const Interval<U>& ivl1, SplitPart lmu) -> Interval<U>;
//! \related Interval \brief Split an interval into its lower and upper half.
template<class U>
inline auto split(const Interval<U>& ivl) -> Pair<Interval<U>, Interval<U>>;

//! \related Interval \brief Equality operator. Tests equality of intervals as geometric objects given information on endpoints.
template<class U>
inline auto operator==(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() == ivl2.upper_bound());
//! \related Interval \brief Inequality operator.
template<class U>
inline auto operator!=(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() != ivl2.upper_bound());

//! \related FloatDPApproximationInterval \related FloatDPExactInterval \brief Allows the over-approximating interval \a ivl to be treated as exact.
Interval<FloatDP> cast_exact(const Interval<FloatDPApproximation>& ivl);
Interval<FloatMP> cast_exact(const Interval<FloatMPApproximation>& ivl);
Interval<FloatDP> cast_exact(const Interval<FloatDPUpperBound>& ivl);
Interval<FloatMP> cast_exact(const Interval<FloatMPUpperBound>& ivl);
Interval<FloatDP> cast_exact_interval(const Interval<FloatDPApproximation>& ivl);
Interval<FloatMP> cast_exact_interval(const Interval<FloatMPApproximation>& ivl);

//! \related FloatDPUpperInterval \brief Computes a common refinement of \a ivl1 and \a ivl2 ivl.e. the intersection.
template<class F>
Interval<UpperBound<F>> refinement(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2);
//! \related FloatDPUpperInterval \brief Tests if \a ivl1 provides a better over-approximation to the exact interval than \a ivl2.
//! ivl.e. \a ivl1 is a subset of \a ivl2.
template<class F>
bool refines(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2);
//! \related FloatDPUpperInterval \brief Tests if two intervals have the same representation.
template<class F>
bool same(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2);

//! \related FloatDPUpperInterval \related FloatBounds \brief Allows the over-approximating interval \a ivl to be treated an over-approximation to a single point.
FloatDPBounds cast_singleton(const Interval<FloatDPUpperBound>& ivl);
/** @brief Construct interval from the supplied arguments. */
FloatDPUpperInterval make_interval(const FloatDPBounds& x);
/** @brief Convert to the singleton representation. */
FloatMPBounds cast_singleton(const Interval<FloatMPUpperBound>& ivl);
/** @brief Construct interval from the supplied arguments. */
FloatMPUpperInterval make_interval(const FloatMPBounds& x);

/** @brief Convert to the singleton representation. */
template<class UB, class PR>
FloatBounds<PR> cast_singleton(const Interval<UB>& ivl, PR pr) {
    return FloatBounds<PR>(ivl.lower_bound(), ivl.upper_bound(), pr);
}

//! \related FloatDPUpperInterval \brief An interval containing the given interval in its interior.
template<class F>
Interval<UpperBound<F>> widen(const Interval<F>& ivl);
template<class F>
Interval<UpperBound<F>> widen(const Interval<UpperBound<F>>& ivl);
template<class F>
Interval<UpperBound<F>> widen(const Interval<UpperBound<F>>& ivl, UpperBound<F> e);
/** @brief Enlarge the enclosure by the supplied amount. */
template<class F>
Interval<UpperBound<F>> widen(const Interval<UpperBound<F>>& ivl, ValidatedUpperNumber e);
/** @brief Enlarge the function domain by the supplied amount. */
template<class F>
Interval<F> widen_domain(const Interval<UpperBound<F>>& ivl);
//! \related LowerInterval<F> \brief An interval contained in the interior of the given interval.
template<class F>
Interval<LowerBound<F>> narrow(const Interval<F>& ivl);
template<class F>
Interval<LowerBound<F>> narrow(const Interval<LowerBound<F>>& ivl);
template<class F>
Interval<LowerBound<F>> narrow(const Interval<LowerBound<F>>& ivl, UpperBound<F> e);
/** @brief Contract the enclosure using the supplied information. */
template<class F>
Interval<UpperBound<F>> narrow(const Interval<LowerBound<F>>& ivl, ValidatedUpperNumber e);

/** @brief Enlarge the function domain by the supplied amount. */
Interval<FloatDP> widen_domain(const Interval<FloatDPUpperBound>& ivl);
/** @brief Return an approximate representation of the domain. */
Interval<FloatDP> approximate_domain(const Interval<FloatDPUpperBound>& ivl);

/** @brief Convert the time interval to the required bound representation. */
inline Interval<FloatDP> to_time_bounds(const Dyadic& tl, const Dyadic& tu) {
    return Interval<FloatDP>(FloatDPLowerBound(tl, DoublePrecision()).raw(), FloatDPLowerBound(tu, DoublePrecision()).raw());
}
/** @brief Convert the time interval to the required bound representation. */
inline Interval<FloatDP> to_time_bounds(const Interval<Dyadic>& ivl) {
    return to_time_bounds(ivl.lower_bound(), ivl.upper_bound());
}

//! \related Interval \brief Read from an input stream.
InputStream& operator>>(InputStream&, Interval<FloatDP>&);

/** @brief Tag selecting an empty interval. */
class EmptyInterval {};
/** @brief Tag selecting the whole real line as an interval. */
class EntireInterval {};

} // namespace Ariadne

#include "interval.inl.hpp"

#endif

