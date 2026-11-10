#include "interval/interval.hpp"
#include "ariadne-testing.hpp"
#include <array>
#include <sstream>
#include <thread>

using namespace Ariadne;

struct RestoreOutputPlaces {
    Nat previous=FloatDP::output_places;
    ~RestoreOutputPlaces() { FloatDP::output_places=previous; }
};

class TestIntervalFormatting : public Testing::ContextFixture {
public:
    using ContextFixture::ContextFixture;

    void test_concurrent_endpoint_formatting_preserves_configuration() {
        RestoreOutputPlaces restore;
        FloatDP::output_places=37u;
        Interval<FloatDP> sample_interval(1.25_x,2.5_x,dp);
        std::ostringstream endpoint_reference;
        endpoint_reference << "{" << sample_interval.lower_bound() << ":" << sample_interval.upper_bound() << "}";
        auto const expected=endpoint_reference.str();
        std::array<unsigned,4> failures{};
        std::array<std::thread,4> workers;
        for(std::size_t i=0u;i<workers.size();++i) {
            workers[i]=std::thread([&,i] {
                for(unsigned iteration=0u;iteration<128u;++iteration) {
                    std::ostringstream output;
                    output << sample_interval;
                    if(output.str()!=expected) ++failures[i];
                }
            });
        }
        for(auto& worker:workers) worker.join();
        for(auto count:failures) ARIADNE_TESTING_EXPECT_EQUAL(count,0u);
        ARIADNE_TESTING_EXPECT_EQUAL(FloatDP::output_places,37u);
    }

    void test_throwing_stream_preserves_numeric_configuration() {
        struct ThrowingBuffer : std::streambuf {
            std::streamsize xsputn(char const*,std::streamsize) override {
                throw std::ios_base::failure("planned output failure");
            }
        } buffer;
        RestoreOutputPlaces restore;
        FloatDP::output_places=37u;
        std::ostream output(&buffer);
        output.exceptions(std::ios::badbit|std::ios::failbit);
        Interval<FloatDP> sample_interval(1.25_x,2.5_x,dp);
        ARIADNE_TESTING_EXPECT_THROWS(std::ios_base::failure,output << sample_interval);
        ARIADNE_TESTING_EXPECT_EQUAL(FloatDP::output_places,37u);
    }
};

int main(int argc,char** argv) {
    ARIADNE_TESTING_SUITE("test_interval_formatting");
    ARIADNE_TESTING_ADD_TEST(TestIntervalFormatting,test_concurrent_endpoint_formatting_preserves_configuration);
    ARIADNE_TESTING_ADD_TEST(TestIntervalFormatting,test_throwing_stream_preserves_numeric_configuration);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc,argv);
}
