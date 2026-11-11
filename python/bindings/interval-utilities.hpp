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

/** @brief Compile-time Python template-name customization. */
template<>
struct PythonTemplateName<Interval> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return "Interval"; }
};

/** @brief Compile-time Python class-name customization. */
template<class X>
struct PythonClassName<Interval<X>> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return python_template_class_name<X>("Interval"); }
};

/** @brief Compile-time Python class-name customization. */
template<>
struct PythonClassName<Interval<FloatDP>> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return "FloatDPExactInterval"; }
};
/** @brief Compile-time Python class-name customization. */
template<>
struct PythonClassName<Interval<FloatDPUpperBound>> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return "FloatDPUpperInterval"; }
};
/** @brief Compile-time Python class-name customization. */
template<>
struct PythonClassName<Interval<FloatDPLowerBound>> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return "FloatDPLowerInterval"; }
};
/** @brief Compile-time Python class-name customization. */
template<>
struct PythonClassName<Interval<FloatDPApproximation>> {
    /** @brief Return the requested element or stored value. */
    static std::string get() { return "FloatDPApproximateInterval"; }
};

/** @brief Write the value to the output stream. */
template<class UB>
OutputStream& operator<<(OutputStream& os, const PythonLiteral<Interval<UB>>& repr) {
    const Interval<UB>& ivl = repr.reference();
    return os << "(" << python_literal(ivl.lower_bound()) << ","
              << python_literal(ivl.upper_bound()) << ")";
}

/** @brief Write the value to the output stream. */
template<class UB>
OutputStream& operator<<(OutputStream& os, const PythonRepresentation<Interval<UB>>& repr) {
    const Interval<UB>& ivl = repr.reference();
    return os << python_class_name<Interval<UB>>() << "("
              << python_literal(ivl.lower_bound()) << ","
              << python_literal(ivl.upper_bound()) << ")";
}

/** @brief Expose conversions to Python. */
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<Approximation<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
    cls.def(pybind11::init<T<UpperBound<F>>>());
    cls.def(pybind11::init<T<LowerBound<F>>>());
}
/** @brief Expose conversions to Python. */
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<LowerBound<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
}
/** @brief Expose conversions to Python. */
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<UpperBound<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
    cls.def(pybind11::init<T<Bounds<F>>>());
}
/** @brief Expose conversions to Python. */
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<Bounds<F>>>& cls) {
    cls.def(pybind11::init<T<F>>());
}
/** @brief Expose conversions to Python. */
template<template<class> class T, class F>
Void export_conversions(pybind11::class_<T<F>>&) {
}

} // namespace Ariadne

#endif
