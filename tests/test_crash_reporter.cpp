
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include <boost/process.hpp>
#include <string>

static constexpr const char* STACKTRACE_FILE_NAME = "stacktrace.txt";

TEST(CrashReporter, CrashingApp) {
    ASSERT_TRUE(std::getenv("APP_PATH")) << "APP_NAME environment variable must be defined!";
    ASSERT_TRUE(std::getenv("PLUGIN_PATH")) << "PLUGIN_PATH environment variable must be defined!";

    boost::process::child c(std::getenv("APP_PATH"));
    c.wait();

    // check correct exit code
    ASSERT_EQ(c.exit_code(), 1);

    // check stacktrace exists
    ASSERT_TRUE(std::filesystem::exists(STACKTRACE_FILE_NAME));
    std::ifstream stacktrace(STACKTRACE_FILE_NAME);
    ASSERT_TRUE(stacktrace.is_open());

    // check (some) data validity
    std::string first_line;
    std::getline(stacktrace, first_line);
    std::string expected_first_line{"Caught signal: "};
    expected_first_line.append(std::to_string(SIGSEGV));
    EXPECT_EQ(first_line, expected_first_line);

    // cleanup
    stacktrace.close();
    std::filesystem::remove(STACKTRACE_FILE_NAME);
}
