#include "ariadne-testing.hpp"
#include "interval/interval.hpp"

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
