/***************************************************************************
 *            interval_module.cpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"

void numeric_module(pybind11::module& module);
void interval_submodule(pybind11::module& module);

void interval_module(pybind11::module& module) {
    numeric_module(module);
    interval_submodule(module);
}
