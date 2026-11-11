/***************************************************************************
 *            interval/interval.inl.hpp
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

/** @file interval/interval.inl.hpp
 *  @brief Inline definitions of interval constructors and arithmetic operations.
 */
namespace Ariadne {

/** @brief Construct split point from the supplied arguments. */
template<class M>
inline M make_split_point(const M& m) { return m; }
/** @brief Construct split point from the supplied arguments. */
template<class F>
inline F make_split_point(const Approximation<F>& am) { return am.raw(); }
/** @brief Construct split point from the supplied arguments. */
template<class F>
inline F make_split_point(const Bounds<F>& bm) { return bm.value(); }
/** @brief Construct split point from the supplied arguments. */
template<class F>
inline F make_split_point(const Ball<F>& bm) { return bm.value(); }

/** @brief Construct the generic numeric value used by the backend workload. */
template<class U, class Y>
inline U create_u(const Y&) { assert(false); }

template<class U>
Interval<U>::Interval() : Interval(EmptyInterval()) {}
template<class U>
Interval<U>::Interval(const EmptyInterval&)
    : Interval(+ExactDouble::infinity(), -ExactDouble::infinity()) {}
template<class U>
Interval<U>::Interval(const UnitInterval&) : Interval(-1, +1) {}
template<class U>
Interval<U>::Interval(const EntireInterval&)
    : Interval(-ExactDouble::infinity(), +ExactDouble::infinity()) {}
template<class U>
Interval<U>::Interval(LowerBoundType l, UpperBoundType u) : _l(l), _u(u) {}

template<class U>
Interval<U> Interval<U>::create_zero() const { return Interval<U>(nul(this->_l), nul(this->_u)); }

template<class U>
SizeOne Interval<U>::dimension() const { return SizeOne(); }
template<class U>
auto Interval<U>::lower_bound() const -> const LowerBoundType& { return _l; }
template<class U>
auto Interval<U>::upper_bound() const -> const UpperBoundType& { return _u; }
template<class U>
auto Interval<U>::centre() const -> CentreType { return hlf(_l + _u); }
template<class U>
auto Interval<U>::midpoint() const -> MidpointType {
    auto m = (hlf(_l + _u));
    return make_split_point(m);
}
template<class U>
auto Interval<U>::radius() const -> RadiusType { return cast_positive(max(this->upper_bound() - this->midpoint(), this->midpoint() - this->lower_bound())); }
template<class U>
auto Interval<U>::width() const -> WidthType { return cast_positive(this->upper_bound() - this->lower_bound()); }

template<class U>
Interval<U> Interval<U>::empty_interval() { return Interval<U>(EmptyInterval()); }
template<class U>
Interval<U> Interval<U>::unit_interval() { return Interval<U>(-1, +1); }
template<class U>
Interval<U> Interval<U>::biinfinite_interval() { return Interval<U>(EntireInterval()); }

template<class U>
auto Interval<U>::is_empty() const -> decltype(declval<L>() > declval<U>()) { return this->_l > this->_u; }
template<class U>
auto Interval<U>::is_bounded() const -> decltype(declval<U>() < declval<L>()) { return Ariadne::is_bounded(*this); }
template<class U>
auto Interval<U>::is_singleton() const -> decltype(declval<L>() == declval<U>()) { return this->_l == this->_u; }

template<class U>
auto Interval<U>::set_lower_bound(LowerBoundType l) -> void { _l = l; }
template<class U>
auto Interval<U>::set_upper_bound(UpperBoundType u) -> void { _u = u; }
template<class U>
auto Interval<U>::set_bounds(LowerBoundType l, UpperBoundType u) -> void {
    _l = l;
    _u = u;
}

/** @brief Write the value to the output stream. */
template<class U>
inline OutputStream& operator<<(OutputStream& os, const Interval<U>& ivl) {
    return os << "{" << ivl.lower_bound() << ":" << ivl.upper_bound() << "}";
}

/** @brief Write the value to the output stream. */
template<>
OutputStream& operator<<(OutputStream& os, const Interval<FloatDP>& ivl);

/** @brief Return the characteristics required to construct compatible values. */
template<class UB>
inline decltype(auto) characteristics(const Interval<UB>& x) {
    if constexpr (HasCharacteristics<UB>) {
        return characteristics(x.upper_bound());
    } else {
        return Tuple<>();
    }
}

/** @brief Return the hyperbolic tangent. */
template<class F>
inline Interval<UpperBound<F>> tanh(const Interval<UpperBound<F>>& ivl) {
    return Interval<UpperBound<F>>(tanh(ivl.lower_bound()), tanh(ivl.upper_bound()));
}

/** @brief Return the lower bound. */
template<class U>
inline auto lower_bound(const Interval<U>& ivl) -> decltype(ivl.lower_bound()) { return ivl.lower_bound(); }
/** @brief Return the upper bound. */
template<class U>
inline auto upper_bound(const Interval<U>& ivl) -> decltype(ivl.upper_bound()) { return ivl.upper_bound(); }
/** @brief Return the centre of the enclosure. */
template<class U>
inline auto centre(const Interval<U>& ivl) -> decltype(ivl.centre()) { return ivl.centre(); }
/** @brief Return the midpoint. */
template<class U>
inline auto midpoint(const Interval<U>& ivl) -> decltype(ivl.midpoint()) { return ivl.midpoint(); }
/** @brief Return the enclosure radius. */
template<class U>
inline auto radius(const Interval<U>& ivl) -> decltype(ivl.radius()) { return ivl.radius(); }
/** @brief Return the difference between the endpoints. */
template<class U>
inline auto width(const Interval<U>& ivl) -> decltype(ivl.width()) { return ivl.width(); }

/** @brief Test whether the set is empty. */
template<class U>
inline auto is_empty(const Interval<U>& ivl) -> decltype(ivl.lower_bound() > ivl.upper_bound()) { return ivl.lower_bound() > ivl.upper_bound(); }
/** @brief Test whether the enclosure consists of a single value. */
template<class U>
inline auto is_singleton(const Interval<U>& ivl) -> decltype(ivl.lower_bound() == ivl.upper_bound()) { return ivl.lower_bound() == ivl.upper_bound(); }

/** @brief Test whether the set has finite bounds. */
template<class U>
inline auto is_bounded(const Interval<U>& ivl) -> decltype(ivl.upper_bound() < ivl.lower_bound()) { return -ivl.lower_bound().raw() < inf && ivl.upper_bound().raw() < inf; }
/** @brief Test whether the set has finite bounds. */
template<>
inline auto is_bounded(const Interval<Real>& ivl) -> decltype(ivl.upper_bound() < ivl.lower_bound()) { return -ivl.lower_bound() < infinity && ivl.upper_bound() < infinity; }

/** @brief Return the selected component. */
template<class U, class X>
inline auto element(const X& x1, const Interval<U>& ivl2) -> decltype(ivl2.lower_bound() <= x1 && ivl2.upper_bound() >= x1) {
    return ivl2.lower_bound() <= x1 && ivl2.upper_bound() >= x1;
}

/** @brief Test containment of the supplied value. */
template<class U, class X>
inline auto contains(const Interval<U>& ivl1, const X& x2) -> decltype(ivl1.lower_bound() <= x2 && ivl1.upper_bound() >= x2) {
    return ivl1.lower_bound() <= x2 && ivl1.upper_bound() >= x2;
}

/** @brief Test equality with the supplied iterator or value. */
template<class U>
inline auto equal(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() == ivl2.upper_bound()) {
    return ivl1.lower_bound() <= ivl2.lower_bound() && ivl1.upper_bound() == ivl2.upper_bound();
}

/** @brief Test whether the first set is contained in the second. */
template<class UB1, class UB2>
inline auto subset(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() <= ivl2.upper_bound()) {
    return ivl1.lower_bound() >= ivl2.lower_bound() && ivl1.upper_bound() <= ivl2.upper_bound();
}

/** @brief Test whether the first set contains the second. */
template<class UB1, class UB2>
inline auto superset(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() >= ivl2.upper_bound()) {
    return ivl1.lower_bound() <= ivl2.lower_bound() && ivl1.upper_bound() >= ivl2.upper_bound();
}

/** @brief Test whether the sets have no common elements. */
template<class UB1, class UB2>
inline auto disjoint(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.lower_bound()) {
    return ivl1.lower_bound() > ivl2.upper_bound() || ivl1.upper_bound() < ivl2.lower_bound();
}

/** @brief Intersect with the supplied set. */
template<class UB1, class UB2>
inline auto intersect(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() >= ivl2.lower_bound()) {
    return ivl1.lower_bound() <= ivl2.upper_bound() && ivl1.upper_bound() >= ivl2.lower_bound();
}

/** @brief Test whether the sets are separated. */
template<class UB1, class UB2>
inline auto separated(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.lower_bound()) {
    return ivl1.lower_bound() > ivl2.upper_bound() || ivl1.upper_bound() < ivl2.lower_bound();
}

/** @brief Test whether the sets overlap. */
template<class UB1, class UB2>
inline auto overlap(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() > ivl2.lower_bound()) {
    return ivl1.lower_bound() < ivl2.upper_bound() && ivl1.upper_bound() > ivl2.lower_bound();
}

/** @brief Test whether the first set lies inside the second. */
template<class UB1, class UB2>
inline auto inside(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() < ivl2.upper_bound()) {
    return ivl1.lower_bound() > ivl2.lower_bound() && ivl1.upper_bound() < ivl2.upper_bound();
}

/** @brief Test whether the first set covers the second. */
template<class UB1, class UB2>
inline auto covers(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> decltype(ivl1.upper_bound() > ivl2.upper_bound()) {
    return ivl1.lower_bound() < ivl2.lower_bound() && ivl1.upper_bound() > ivl2.upper_bound();
}

/** @brief Return the intersection of the operands. */
template<class UB1, class UB2>
inline auto
    intersection(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> Interval<decltype(min(declval<UB1>(), declval<UB2>()))> {
    return Interval<decltype(min(declval<UB1>(), declval<UB2>()))>(max(ivl1.lower_bound(), ivl2.lower_bound()), min(ivl1.upper_bound(), ivl2.upper_bound()));
}

/** @brief Return the smallest representable enclosure containing the operands. */
template<class UB1, class UB2>
inline auto
    hull(const Interval<UB1>& ivl1, const Interval<UB2>& ivl2) -> Interval<decltype(max(declval<UB1>(), declval<UB2>()))> {
    typedef decltype(max(declval<UB1>(), declval<UB2>())) UB0;
    return Interval<UB0>(min(ivl1.lower_bound(), ivl2.lower_bound()), max(ivl1.upper_bound(), ivl2.upper_bound()));
}

/** @brief Return the smallest representable enclosure containing the operands. */
template<class U, class X>
inline auto
    hull(const Interval<U>& ivl1, X x2) -> Interval<decltype(max(declval<U>(), declval<X>()))> {
    return Interval<decltype(max(declval<U>(), declval<X>()))>(min(ivl1.lower_bound(), x2), max(ivl1.upper_bound(), x2));
}

/** @brief Return the smallest representable enclosure containing the operands. */
template<class U, class X>
inline auto
    hull(X x1, const Interval<U>& ivl2) -> Interval<decltype(max(declval<U>(), declval<X>()))> {
    return Interval<decltype(max(declval<U>(), declval<X>()))>(min(x1, ivl2.lower_bound()), max(x1, ivl2.upper_bound()));
}

/** @brief Split the domain or model along the selected coordinate. */
template<class U>
inline auto split(const Interval<U>& ivl, SplitPart lmu) -> Interval<U> {
    auto cc = hlf(ivl.lower_bound() + ivl.upper_bound());
    if (lmu == SplitPart::LOWER) {
        return Interval<U>(ivl.lower_bound(), make_split_point(cc));
    } else if (lmu == SplitPart::UPPER) {
        return Interval<U>(make_split_point(cc), ivl.upper_bound());
    } else {
        auto cl = (3 * ivl.lower_bound() + ivl.upper_bound()) * 0.25_x;
        auto cu = (ivl.lower_bound() + 3 * ivl.upper_bound()) * 0.25_x;
        return Interval<U>(make_split_point(cl), make_split_point(cu));
    }
}

/** @brief Split the domain or model along the selected coordinate. */
template<class U>
inline auto split(const Interval<U>& ivl) -> Pair<Interval<U>, Interval<U>> {
    auto c = make_split_point(hlf(ivl.lower_bound() + ivl.upper_bound()));
    return std::make_pair(Interval<U>(ivl.lower_bound(), c), Interval<U>(c, ivl.upper_bound()));
}

/** @brief Test equality of the operands. */
template<class U>
inline auto operator==(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() == ivl2.upper_bound()) {
    return ivl1.lower_bound() == ivl2.lower_bound() && ivl1.upper_bound() == ivl2.upper_bound();
}
/** @brief Test inequality of the operands. */
template<class U>
inline auto operator!=(const Interval<U>& ivl1, const Interval<U>& ivl2) -> decltype(ivl1.upper_bound() != ivl2.upper_bound()) {
    return ivl1.lower_bound() != ivl2.lower_bound() || ivl1.upper_bound() != ivl2.upper_bound();
}

/** @brief Return the common refinement of the enclosures. */
template<class U1, class U2>
inline decltype(auto) refinement(const Interval<U1>& ivl1, const Interval<U2>& ivl2) {
    return make_interval(refinement(ivl1.lower_bound(), ivl2.lower_bound()), refinement(ivl1.upper_bound(), ivl2.upper_bound()));
}
/** @brief Test whether the first enclosure is contained in the second. */
template<class U1, class U2>
inline bool refines(const Interval<U1>& ivl1, const Interval<U2>& ivl2) {
    return refines(ivl1.lower_bound(), ivl2.lower_bound()) and refines(ivl1.upper_bound(), ivl2.upper_bound());
}
/** @brief Test whether the operands have the same representation. */
template<class U1, class U2>
inline bool same(const Interval<U1>& ivl1, const Interval<U2>& ivl2) {
    return same(ivl1.lower_bound(), ivl2.lower_bound()) and same(ivl1.upper_bound(), ivl2.upper_bound());
}

/** @brief Return the common refinement of the enclosures. */
template<class F>
inline Interval<UpperBound<F>> refinement(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2) {
    return Interval<UpperBound<F>>(max(ivl1.lower_bound().raw(), ivl2.lower_bound().raw()), min(ivl1.upper_bound().raw(), ivl2.upper_bound().raw()));
}
/** @brief Test whether the first enclosure is contained in the second. */
template<class F>
inline Bool refines(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2) {
    return ivl1.lower_bound().raw() >= ivl2.lower_bound().raw() && ivl1.upper_bound().raw() <= ivl2.upper_bound().raw();
}
/** @brief Test whether the operands have the same representation. */
template<class F>
inline Bool same(const Interval<UpperBound<F>>& ivl1, const Interval<UpperBound<SelfType<F>>>& ivl2) {
    return ivl1.lower_bound().raw() == ivl2.lower_bound().raw() && ivl1.upper_bound().raw() == ivl2.upper_bound().raw();
}

/** @brief Enlarge the enclosure by the supplied amount. */
template<class F>
inline Interval<UpperBound<F>> widen(const Interval<UpperBound<F>>& ivl, UpperBound<F> e) {
    return Interval<UpperBound<F>>(ivl.lower_bound() - e, ivl.upper_bound() + e);
}
/** @brief Enlarge the enclosure by the supplied amount. */
template<class F>
inline Interval<UpperBound<F>> widen(const Interval<UpperBound<F>>& ivl) {
    return widen(ivl, UpperBound<F>(F::min(ivl.upper_bound().precision())));
}
/** @brief Enlarge the enclosure by the supplied amount. */
template<class F>
inline Interval<UpperBound<F>> widen(const Interval<F>& ivl) {
    return widen(Interval<UpperBound<F>>(ivl), UpperBound<F>(F::min(ivl.upper_bound().precision())));
}

/** @brief Contract the enclosure using the supplied information. */
template<class F>
inline Interval<LowerBound<F>> narrow(const Interval<LowerBound<F>>& ivl, UpperBound<F> e) {
    return Interval<LowerBound<F>>(ivl.lower_bound() + e, ivl.upper_bound() - e);
}
/** @brief Contract the enclosure using the supplied information. */
template<class F>
inline Interval<LowerBound<F>> narrow(const Interval<LowerBound<F>>& ivl) {
    return narrow(ivl, UpperBound<F>(F::min(ivl.upper_bound().precision())));
}
/** @brief Contract the enclosure using the supplied information. */
template<class F>
inline Interval<LowerBound<F>> narrow(const Interval<F>& ivl) {
    return narrow(Interval<LowerBound<F>>(ivl), UpperBound<F>(F::min(ivl.upper_bound().precision())));
}

/** @brief Convert to the exact representation. */
inline Interval<FloatDP> cast_exact(const Interval<FloatDPApproximation>& ivl) {
    return Interval<FloatDP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}
/** @brief Convert to the exact representation. */
inline Interval<FloatMP> cast_exact(const Interval<FloatMPApproximation>& ivl) {
    return Interval<FloatMP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}
/** @brief Convert to the exact representation. */
inline Interval<FloatDP> cast_exact(const Interval<FloatDPUpperBound>& ivl) {
    return Interval<FloatDP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}
/** @brief Convert to the exact representation. */
inline Interval<FloatMP> cast_exact(const Interval<FloatMPUpperBound>& ivl) {
    return Interval<FloatMP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}
/** @brief Convert to the exact interval representation. */
inline Interval<FloatDP> cast_exact_interval(const Interval<FloatDPApproximation>& ivl) {
    return Interval<FloatDP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}
/** @brief Convert to the exact interval representation. */
inline Interval<FloatMP> cast_exact_interval(const Interval<FloatMPApproximation>& ivl) {
    return Interval<FloatMP>(ivl.lower_bound().raw(), ivl.upper_bound().raw());
}

/** @brief Return a lower bound on the absolute value. */
inline FloatDPLowerBound mig(const FloatDPUpperInterval& ivl) { return mig(cast_singleton(ivl)); }
/** @brief Return a lower bound on the absolute value. */
inline FloatMPLowerBound mig(const FloatMPUpperInterval& ivl) { return mig(cast_singleton(ivl)); }

} // namespace Ariadne
