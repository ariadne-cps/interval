/***************************************************************************
 *            test_interval_testing.cpp
 *
 *  Copyright  2026  Ariadne contributors
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne. If not, see <https://www.gnu.org/licenses/>.
 */

#include "interval/interval.hpp"

#include "ariadne-testing.hpp"

using namespace Ariadne;
using namespace Ariadne::Testing;

int main(int argc, char** argv) {
    Suite suite("interval testing integration");
    suite.add("exact interval construction and ordering", [](Context& context) {
        ExactIntervalType interval(1, 2);
        ARIADNE_TESTING_PROVE_CTX(context,
                                  interval.lower_bound() < interval.upper_bound());
        ARIADNE_TESTING_EXPECT_EQUAL_CTX(context,
                                         interval.lower_bound(), ExactIntervalType(1, 2).lower_bound());
        ARIADNE_TESTING_EXPECT_EQUAL_CTX(context,
                                         interval.upper_bound(), ExactIntervalType(1, 2).upper_bound());
    });
    suite.add("empty interval and hull", [](Context& context) {
        ExactIntervalType empty(EmptyInterval{});
        ARIADNE_TESTING_PROVE_CTX(context, empty.is_empty());
        ExactIntervalType interval(1, 2);
        ARIADNE_TESTING_EXPECT_NO_THROW_CTX(context, hull(interval, interval));
    });
    return run_suite(suite, argc, argv);
}
