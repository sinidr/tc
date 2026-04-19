#include "plugin/plugin.h"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

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
        if (!handle_) {
            std::cerr << "Failed to load plugin: error code " << GetLastError() << std::endl;
        }
#else
        handle_ = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle_) {
            std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        }
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

    PluginLoader(PluginLoader&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }

    PluginLoader& operator=(PluginLoader&& other) noexcept {
        if (this != &other) {
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

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

std::string find_plugin(const char* path, std::string_view name) {
    auto exe_dir = std::filesystem::path(path).parent_path();
    if (exe_dir.empty()) exe_dir = ".";

    std::string plugin_file_name;
#ifdef _WIN32
    plugin_file_name.append(name);
    plugin_file_name.append(".dll");
#elif defined(__APPLE__)
    plugin_file_name.append("lib");
    plugin_file_name.append(name);
    plugin_file_name.append(".dylib");
#else
    plugin_file_name.append("lib");
    plugin_file_name.append(name);
    plugin_file_name.append(".so");
#endif
    
    // Build layout: plugin sits next to the executable.
    // Install layout (Linux/macOS): plugin lives in ../lib/ relative to bin/.
    for (auto&& candidate : {
             exe_dir / plugin_file_name,
             exe_dir / ".." / "lib" / plugin_file_name,
         }) {
        if (std::filesystem::exists(candidate))
            return std::filesystem::canonical(candidate).string();
    }

    return (exe_dir / plugin_file_name).string();
}

class Plugin {
public:

    explicit Plugin(std::string const& file)
        : loader_(PluginLoader(file))
    {
        init_ = loader_.get_symbol<plugin_init_fn>("plugin_init");
        get_name_ = loader_.get_symbol<plugin_get_name_fn>("plugin_get_name");
        add_ = loader_.get_symbol<plugin_add_fn>("plugin_add");
    }

    Plugin(Plugin const&) = delete;
    Plugin& operator=(Plugin const&) = delete;
    Plugin(Plugin&&) = default;
    Plugin& operator=(Plugin&&) = default;
    ~Plugin() = default;

    auto error() const -> std::optional<std::string> {
        if (!loader_.is_loaded()) {
            return "Could not load the plugin library.";
        }
        // note: plugin_add is not mandatory (I made that rule up)
        if (!init_ || !get_name_) {
            return "Failed to resolve plugin symbols.";
        }
        return std::nullopt;
    }

    int init() const {
        return init_();
    }

    auto get_name() const -> const char* {
        return get_name_();
    }

    bool has_add() const {
        return !!add_;
    }

    int add(int a, int b) const {
        if (!has_add()) {
            throw std::runtime_error("Plugin does not have a plugin_add!");
        }
        return add_(a, b);
    }

private:
    PluginLoader loader_;
    plugin_init_fn init_{nullptr};
    plugin_get_name_fn get_name_{nullptr};
    plugin_add_fn add_{nullptr};
};

int main(int argc, char* argv[]) {

    constexpr std::array<const char*, 2> plugin_names = {{ "plugin", "crash_reporter" }};

    std::vector<Plugin> plugins;
    for (const auto plugin_name: plugin_names) {
        auto plugin_path = find_plugin(argv[0], plugin_name);
        std::cout << "Loading plugin from: " << plugin_path << std::endl;
        plugins.emplace_back(plugin_path);
    }

    for (const auto& plugin: plugins) {
        auto error = plugin.error();
        if (error) {
            std::cerr << error.value() << std::endl;
            return EXIT_FAILURE;
        }
    }

    for (const auto& plugin: plugins) {
        int rc = plugin.init();
        if (rc != 0) {
            std::cerr << "plugin_init failed with code " << rc << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Plugin name : " << plugin.get_name() << std::endl;
        if (plugin.has_add()) {
            std::cout << "plugin_add(3, 4) = " << plugin.add(3, 4) << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
