#include <absent/and_then.h>
#include <absent/support/execution_status.h>

#include <string>

#include <catch2/catch_all.hpp>

using namespace rvarago::absent;

SCENARIO("absent can be used for multiple error handling", "[execution_status]") {

    bool is_set = false;
    auto const set_flag = [&is_set]() -> support::execution_status {
        is_set = true;
        return support::success;
    };

    WHEN("when initial nullable is failed") {

        support::execution_status failed = support::failure;

        THEN("do nothing and return a failed nullable to stop the chaining") {
            auto then_failed = failed >> support::sink(set_flag);

            CHECK(!then_failed.has_value());
            CHECK(!is_set);
        }
    }

    WHEN("when initial nullable is ok") {

        support::execution_status ok = support::success;

        THEN("run the function that triggers a side-effect and return an ok nullable") {
            auto then_ok = ok >> support::sink(set_flag);

            CHECK(then_ok.has_value());
            CHECK(is_set);
        }
    }
}