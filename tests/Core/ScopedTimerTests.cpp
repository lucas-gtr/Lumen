#include <gtest/gtest.h>
#include <sstream>
#include <thread>
#include <chrono>
#include "Core/ScopedTimer.hpp"

namespace {

std::string capturePrintStats() {
    std::ostringstream oss;
    std::streambuf* old_cout_buf = std::cout.rdbuf(oss.rdbuf());
    ScopedTimer::PrintStats();
    std::cout.rdbuf(old_cout_buf);
    return oss.str();
}

} // namespace

TEST(ScopedTimerTest, StopManuallyRecordsTime) {
    const std::string name = "ManualStop";
    {
        ScopedTimer timer(name);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timer.stop();
    }

    std::string stats_output = capturePrintStats();
    EXPECT_NE(stats_output.find(name), std::string::npos);
}

TEST(ScopedTimerTest, RAIIStopsTimerAutomatically) {
    const std::string name = "RAIIStop";
    {
        ScopedTimer timer(name);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    std::string stats_output = capturePrintStats();
    EXPECT_NE(stats_output.find(name), std::string::npos);
}

TEST(ScopedTimerTest, MultipleCallsAccumulateStats) {
    const std::string name = "Accumulated";

    for (int i = 0; i < 3; ++i) {
        ScopedTimer timer(name);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    std::string stats_output = capturePrintStats();
    EXPECT_NE(stats_output.find(name), std::string::npos);
    EXPECT_NE(stats_output.find("calls: 3"), std::string::npos);
}

TEST(ScopedTimerTest, StopIsIdempotent) {
    const std::string name = "StopTwice";
    {
        ScopedTimer timer(name);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        timer.stop();
        timer.stop(); // appel redondant
    }

    std::string stats_output = capturePrintStats();
    EXPECT_NE(stats_output.find(name), std::string::npos);
}

