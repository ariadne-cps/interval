/***************************************************************************
 *            interval-utilities.hpp
 *
 *  Copyright  2026  Pieter Collins
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Interval. If not, see <https://www.gnu.org/licenses/>.
 */

/** @file interval-utilities.hpp
 *  @brief Python binding helpers and interval type-name specializations.
 */
#ifndef ARIADNE_PYTHON_INTERVAL_UTILITIES_HPP
#define ARIADNE_PYTHON_INTERVAL_UTILITIES_HPP

#include "interval/interval.hpp"
#include "numeric_submodule.hpp"

namespace Ariadne {

template<>
struct PythonTemplateName<Interval> {
    static std::string get() { return "Interval"; }
};

template<class X>
struct PythonClassName<Interval<X>> {
    static std::string get() { return python_template_class_name<X>("Interval"); }
};

template<>
struct PythonClassName<Interval<FloatDP>> {
    static std::string get() { return "FloatDPExactInterval"; }
};
template<>
struct PythonClassName<Interval<FloatDPUpperBound>> {
    static std::string get() { return "FloatDPUpperInterval"; }
};
template<>
struct PythonClassName<Interval<FloatDPLowerBound>> {
    static std::string get() { return "FloatDPLowerInterval"; }
};
template<>
struct PythonClassName<Interval<FloatDPApproximation>> {
    static std::string get() { return "FloatDPApproximateInterval"; }
};

template<class UB>
OutputStream& operator<<(OutputStream& os, const PythonLiteral<Interval<UB>>& repr) {
    const Interval<UB>& ivl = repr.reference();
    return os << "(" << python_literal(ivl.lower_bound()) << ","
              << python_literal(ivl.upper_bound()) << ")";
}

template<class UB>
OutputStream& operator<<(OutputStream& os, const PythonRepresentation<Interval<UB>>& repr) {
    const Interval<UB>& ivl = repr.reference();
    return os << python_class_name<Interval<UB>>() << "("
              << python_literal(ivl.lower_bound()) << ","
              << python_literal(ivl.upper_bound()) << ")";
}

template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<Approximation<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
    cls.def(pybind11::init<T<UpperBound<F>>>());
    cls.def(pybind11::init<T<LowerBound<F>>>());
}
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<LowerBound<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
}
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<UpperBound<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
}
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<Bounds<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
}
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<F>>&) {
}

} // namespace Ariadne

#endif
