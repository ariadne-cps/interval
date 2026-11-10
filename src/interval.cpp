/***************************************************************************
 *            geometry/interval.cpp
 *
 *  Copyright  2013-20  Pieter Collins
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

#include "interval/interval.hpp"

#include "numeric/dyadic.hpp"

namespace Ariadne {

template<>
OutputStream& operator<<(OutputStream& os, const Interval<FloatDP>& ivl) {
    // FloatDP insertion uses repr(), whose precision is explicit. Changing
    // the legacy global output_places cannot affect this output and races
    // with other formatters (and leaked the setting if the stream threw).
    return os << "{" << ivl.lower_bound() << ":" << ivl.upper_bound() << "}";
}

Interval<FloatDP> widen_domain(const Interval<FloatDPUpperBound>& ivl) {
    auto rnd = FloatDP::get_rounding_mode();
    FloatDP::set_rounding_mode(upward);
    volatile float min = std::numeric_limits<float>::min();
    volatile double neg_l = (-ivl.lower_bound()).get_d();
    volatile double l = -neg_l;
    volatile double u = ivl.upper_bound().get_d();
    volatile float neg_rl = static_cast<float>(neg_l);
    volatile float ru = static_cast<float>(u);
    if (l == u) {
        neg_rl = neg_rl + min;
        ru = ru + min;
    }
    volatile float rl = -neg_rl;
    Interval<FloatDP> res(ExactDouble(rl), ExactDouble(ru), dp);
    FloatDP::set_rounding_mode(rnd);
    return res;
}

Interval<FloatDP> approximate_domain(const Interval<FloatDPUpperBound>& ivl) {
    auto rnd = FloatDP::get_rounding_mode();
    FloatDP::set_rounding_mode(to_nearest);
    volatile float eps = std::numeric_limits<float>::epsilon();
    volatile double l = ivl.lower_bound().get_d();
    volatile double u = ivl.upper_bound().get_d();
    volatile float rl = static_cast<float>(l);
    volatile float ru = static_cast<float>(u);
    if (rl == ru) {
        rl = rl - (rl * eps);
        ru = ru + (ru * eps);
    }
    Interval<FloatDP> res(ExactDouble(rl), ExactDouble(ru), dp);
    FloatDP::set_rounding_mode(rnd);
    return res;
}

InputStream&
    operator>>(InputStream& is, Interval<FloatDP>& ivl) {
    FloatDP l(dp), u(dp);
    char cl, cm, cr;
    is >> cl >> l >> cm >> u >> cr;
    ARIADNE_ASSERT(not is.fail());
    ARIADNE_ASSERT(cl == '[' || cl == '(');
    ARIADNE_ASSERT(cm == ':' || cm == ',' || cm == ';');
    ARIADNE_ASSERT(cr == ']' || cr == ')');
    ivl.set_bounds(FloatDP(l), FloatDP(u));
    return is;
}

template<>
FloatDPUpperInterval UpperIntervalFactory<FloatDP>::create(const ValidatedNumber& y) const {
    return FloatDPUpperInterval(FloatDPBounds(y, this->_precision));
}

FloatDPBounds cast_singleton(const FloatDPUpperInterval& ivl) {
    return FloatDPBounds(ivl.lower_bound(), ivl.upper_bound());
}
FloatDPUpperInterval make_interval(const FloatDPBounds& x) {
    return FloatDPUpperInterval(x.lower(), x.upper());
}

FloatDPUpperInterval max(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(max(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatDPUpperInterval min(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(min(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatDPUpperInterval abs(const FloatDPUpperInterval& ivl) {
    return make_interval(abs(cast_singleton(ivl)));
}
FloatDPUpperInterval nul(const FloatDPUpperInterval& ivl) {
    return make_interval(nul(cast_singleton(ivl)));
}
FloatDPUpperInterval pos(const FloatDPUpperInterval& ivl) {
    return make_interval(pos(cast_singleton(ivl)));
}
FloatDPUpperInterval neg(const FloatDPUpperInterval& ivl) {
    return make_interval(neg(cast_singleton(ivl)));
}
FloatDPUpperInterval sqr(const FloatDPUpperInterval& ivl) {
    return make_interval(sqr(cast_singleton(ivl)));
}
FloatDPUpperInterval hlf(const FloatDPUpperInterval& ivl) {
    return make_interval(hlf(cast_singleton(ivl)));
}
FloatDPUpperInterval rec(const FloatDPUpperInterval& ivl) {
    return make_interval(rec(cast_singleton(ivl)));
}

FloatDPUpperInterval add(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(add(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatDPUpperInterval sub(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(sub(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatDPUpperInterval mul(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(mul(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatDPUpperInterval div(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return make_interval(div(cast_singleton(ivl1), cast_singleton(ivl2)));
}

FloatDPUpperInterval fma(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2, FloatDPUpperInterval ivl3) {
    return make_interval(fma(cast_singleton(ivl1), cast_singleton(ivl2), cast_singleton(ivl3)));
}

FloatDPUpperInterval pow(const FloatDPUpperInterval& ivl, Nat m) {
    return make_interval(pow(cast_singleton(ivl), m));
}
FloatDPUpperInterval pow(const FloatDPUpperInterval& ivl, Int n) {
    return make_interval(pow(cast_singleton(ivl), n));
}

FloatDPUpperInterval sqrt(const FloatDPUpperInterval& ivl) {
    // Defining sqrt(I)=hull{x:x^2 in I}, if I=[a,b] with b>=0, then sqrt(I)=[-sqrt(b),+sqrt(b)]
    if (ivl.upper_bound().raw() < 0) {
        return FloatDPUpperInterval::empty_interval();
    } else {
        FloatDP u = sqrt(up, ivl.upper_bound().raw());
        return FloatDPUpperInterval(-u, +u);
    }
}
FloatDPUpperInterval exp(const FloatDPUpperInterval& ivl) {
    return make_interval(exp(cast_singleton(ivl)));
}
FloatDPUpperInterval log(const FloatDPUpperInterval& ivl) {
    if (ivl.upper_bound().raw() <= 0) {
        return FloatDPUpperInterval::empty_interval();
    } else if (ivl.lower_bound().raw() <= 0) {
        return FloatDPUpperInterval(FloatDP(-inf, dp), log(up, ivl.upper_bound().raw()));
    } else {
        return make_interval(log(cast_singleton(ivl)));
    }
}
FloatDPUpperInterval sin(const FloatDPUpperInterval& ivl) {
    return make_interval(sin(cast_singleton(ivl)));
}
FloatDPUpperInterval cos(const FloatDPUpperInterval& ivl) {
    return make_interval(cos(cast_singleton(ivl)));
}
FloatDPUpperInterval tan(const FloatDPUpperInterval& ivl) {
    return make_interval(tan(cast_singleton(ivl)));
}
FloatDPUpperInterval asin(const FloatDPUpperInterval& ivl) {
    return make_interval(asin(cast_singleton(ivl)));
}
FloatDPUpperInterval acos(const FloatDPUpperInterval& ivl) {
    return make_interval(acos(cast_singleton(ivl)));
}
FloatDPUpperInterval atan(const FloatDPUpperInterval& ivl) {
    return make_interval(atan(cast_singleton(ivl)));
}

FloatDPError mag(const FloatDPUpperInterval& ivl) {
    return mag(cast_singleton(ivl));
}

ValidatedKleenean eq(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return eq(cast_singleton(ivl1), cast_singleton(ivl2));
}
ValidatedKleenean lt(const FloatDPUpperInterval& ivl1, const FloatDPUpperInterval& ivl2) {
    return lt(cast_singleton(ivl1), cast_singleton(ivl2));
}

FloatMPBounds cast_singleton(const FloatMPUpperInterval& ivl) {
    return FloatMPBounds(ivl.lower_bound(), ivl.upper_bound());
}
FloatMPUpperInterval make_interval(const FloatMPBounds& x) {
    return FloatMPUpperInterval(x.lower(), x.upper());
}

template<>
FloatMPUpperInterval UpperIntervalFactory<FloatMP>::create(const ValidatedNumber& y) const {
    return FloatMPUpperInterval(FloatMPBounds(y, this->_precision));
}

FloatMPUpperInterval max(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(max(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatMPUpperInterval min(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(min(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatMPUpperInterval abs(const FloatMPUpperInterval& ivl) {
    return make_interval(abs(cast_singleton(ivl)));
}
FloatMPUpperInterval nul(const FloatMPUpperInterval& ivl) {
    return make_interval(nul(cast_singleton(ivl)));
}
FloatMPUpperInterval pos(const FloatMPUpperInterval& ivl) {
    return make_interval(pos(cast_singleton(ivl)));
}
FloatMPUpperInterval neg(const FloatMPUpperInterval& ivl) {
    return make_interval(neg(cast_singleton(ivl)));
}
FloatMPUpperInterval sqr(const FloatMPUpperInterval& ivl) {
    return make_interval(sqr(cast_singleton(ivl)));
}
FloatMPUpperInterval hlf(const FloatMPUpperInterval& ivl) {
    return make_interval(hlf(cast_singleton(ivl)));
}
FloatMPUpperInterval rec(const FloatMPUpperInterval& ivl) {
    return make_interval(rec(cast_singleton(ivl)));
}

FloatMPUpperInterval add(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(add(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatMPUpperInterval sub(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(sub(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatMPUpperInterval mul(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(mul(cast_singleton(ivl1), cast_singleton(ivl2)));
}
FloatMPUpperInterval div(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return make_interval(div(cast_singleton(ivl1), cast_singleton(ivl2)));
}

FloatMPUpperInterval fma(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2, FloatMPUpperInterval ivl3) {
    return make_interval(fma(cast_singleton(ivl1), cast_singleton(ivl2), cast_singleton(ivl3)));
}

FloatMPUpperInterval pow(const FloatMPUpperInterval& ivl, Nat m) {
    return make_interval(pow(cast_singleton(ivl), m));
}
FloatMPUpperInterval pow(const FloatMPUpperInterval& ivl, Int n) {
    return make_interval(pow(cast_singleton(ivl), n));
}

FloatMPUpperInterval sqrt(const FloatMPUpperInterval& ivl) {
    // Defining sqrt(I)=hull{x:x^2 in I}, if I=[a,b] with b>=0, then sqrt(I)=[-sqrt(b),+sqrt(b)]
    auto pr = ivl.upper_bound().precision();
    auto inf_ = FloatMP::inf(pr);
    if (ivl.upper_bound().raw() < 0) {
        return FloatMPUpperInterval(+inf_, -inf_);
    } else {
        FloatMP u = sqrt(up, ivl.upper_bound().raw());
        return FloatMPUpperInterval(-u, +u);
    }
}
FloatMPUpperInterval exp(const FloatMPUpperInterval& ivl) {
    return make_interval(exp(cast_singleton(ivl)));
}
FloatMPUpperInterval log(const FloatMPUpperInterval& ivl) {
    auto pr = ivl.upper_bound().precision();
    auto inf_ = FloatMP::inf(pr);
    if (ivl.upper_bound().raw() <= 0) {
        return FloatMPUpperInterval(+inf_, -inf_);
    } else if (ivl.lower_bound().raw() <= 0) {
        return FloatMPUpperInterval(-inf_, log(up, ivl.upper_bound().raw()));
    } else {
        return make_interval(log(cast_singleton(ivl)));
    }
}
FloatMPUpperInterval sin(const FloatMPUpperInterval& ivl) {
    return make_interval(sin(cast_singleton(ivl)));
}
FloatMPUpperInterval cos(const FloatMPUpperInterval& ivl) {
    return make_interval(cos(cast_singleton(ivl)));
}
FloatMPUpperInterval tan(const FloatMPUpperInterval& ivl) {
    return make_interval(tan(cast_singleton(ivl)));
}
FloatMPUpperInterval asin(const FloatMPUpperInterval& ivl) {
    return make_interval(asin(cast_singleton(ivl)));
}
FloatMPUpperInterval acos(const FloatMPUpperInterval& ivl) {
    return make_interval(acos(cast_singleton(ivl)));
}
FloatMPUpperInterval atan(const FloatMPUpperInterval& ivl) {
    return make_interval(atan(cast_singleton(ivl)));
}

FloatMPError mag(const FloatMPUpperInterval& ivl) {
    return mag(cast_singleton(ivl));
}

ValidatedKleenean eq(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return eq(cast_singleton(ivl1), cast_singleton(ivl2));
}
ValidatedKleenean lt(const FloatMPUpperInterval& ivl1, const FloatMPUpperInterval& ivl2) {
    return lt(cast_singleton(ivl1), cast_singleton(ivl2));
}

ApproximateKleenean eq(const FloatDPApproximateInterval& ivl1, const FloatDPApproximateInterval& ivl2) {
    return ivl1.lower_bound() == ivl2.lower_bound() && ivl1.upper_bound() == ivl2.upper_bound();
}
ApproximateKleenean eq(const FloatMPApproximateInterval& ivl1, const FloatMPApproximateInterval& ivl2) {
    return ivl1.lower_bound() == ivl2.lower_bound() && ivl1.upper_bound() == ivl2.upper_bound();
}

template<>
String class_name<FloatDPUpperInterval>() { return "FloatDPUpperInterval"; }
template<>
String class_name<FloatMPUpperInterval>() { return "FloatMPUpperInterval"; }
template<>
String class_name<Interval<FloatDP>>() { return "FloatDPInterval"; }
template<>
String class_name<Interval<FloatMP>>() { return "FloatDPInterval"; }
template<>
String class_name<Interval<Real>>() { return "RealInterval"; }

} // namespace Ariadne
