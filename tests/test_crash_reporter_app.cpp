
#include "plugin_loader.h"
#include <cstdlib>

void i_cause_a_segfault() {
    int* ptr = reinterpret_cast<int*>(42); // NOLINT
    *ptr = 0;
}

int main() {
    PluginLoader loader{get_plugin_path()};
    if (!loader.is_loaded()) {
        return 2;
    }

    auto init = loader.get_symbol<plugin_init_fn>("plugin_init");
    if (!init) {
        return 3;
    }

    auto rc = init();
    if (rc != 0) {
        return 4;
    }

    i_cause_a_segfault();
    return EXIT_SUCCESS;
}
