
#define PLUGIN_BUILDING

#include "plugin/plugin.h"

extern "C" {

PLUGIN_API int plugin_init(void) {
    return 0;
}

PLUGIN_API const char* plugin_get_name(void) {
    return "test_plugin";
}

PLUGIN_API int plugin_add(int a, int b) {
    return a - b;
}

}
