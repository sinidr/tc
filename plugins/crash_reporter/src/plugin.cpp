#include "plugin/plugin.h"

#include <cpptrace/basic.hpp>
#include <cpptrace/cpptrace.hpp>
#include <csignal>
#include <cstdlib>
#include <fstream>

namespace {

void signal_handler(int sig) {
    auto trace = cpptrace::generate_trace();

    std::ofstream out("stacktrace.txt");
    out << "Caught signal: " << sig << "\n";
    out << trace << std::endl;
    out.close();

    std::_Exit(1);
}

} // namespace

extern "C" {

PLUGIN_API int plugin_init(void) {
    std::ignore = std::signal(SIGSEGV, signal_handler);
    return 0;
}

PLUGIN_API const char* plugin_get_name(void) {
    return "crash_reporter";
}
}
