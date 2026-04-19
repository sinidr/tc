
#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

using plugin_init_fn = int (*)();
using plugin_get_name_fn = const char* (*)();
using plugin_add_fn = int (*)(int, int);

class PluginLoader {
public:
    explicit PluginLoader(const std::string& path) {
#ifdef _WIN32
        handle_ = LoadLibraryA(path.c_str());
#else
        handle_ = dlopen(path.c_str(), RTLD_LAZY);
#endif
    }

    ~PluginLoader() {
        if (!handle_) return;
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(handle_));
#else
        dlclose(handle_);
#endif
    }

    PluginLoader(const PluginLoader&) = delete;
    PluginLoader& operator=(const PluginLoader&) = delete;

    template <typename T>
    T get_symbol(const char* name) const {
        if (!handle_) return nullptr;
#ifdef _WIN32
        return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(handle_), name));
#else
        return reinterpret_cast<T>(dlsym(handle_, name));
#endif
    }

    [[nodiscard]] bool is_loaded() const { return handle_ != nullptr; }

private:
    void* handle_ = nullptr;
};

std::string get_plugin_path() {
    const char* env = std::getenv("PLUGIN_PATH");
    if (env) return env;

    auto exe_dir = std::filesystem::path(".");
#ifdef _WIN32
    return (exe_dir / "plugin.dll").string();
#elif defined(__APPLE__)
    return (exe_dir / "libplugin.dylib").string();
#else
    return (exe_dir / "libplugin.so").string();
#endif
}
