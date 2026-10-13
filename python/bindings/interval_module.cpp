/***************************************************************************
 *            interval_module.cpp
 *
 *  Copyright  2026  Pieter Collins
 *
 ****************************************************************************/

#include "pybind11.hpp"

void interval_submodule(pybind11::module& module);

void interval_module(pybind11::module& module) {
    interval_submodule(module);
}
