#include <gtest/gtest.h>

#include "plugin_loader.h"

namespace {


class PluginTest : public ::testing::Test {
protected:
    void SetUp() override {
        loader_ = std::make_unique<PluginLoader>(get_plugin_path());
        ASSERT_TRUE(loader_->is_loaded()) << "Plugin library not found. "
            "Set PLUGIN_PATH or run from the build/bin directory.";
    }

    void TearDown() override {
        loader_.reset();
    }

    std::unique_ptr<PluginLoader> loader_;
};

TEST_F(PluginTest, PluginLoadsSuccessfully) {
    EXPECT_TRUE(loader_->is_loaded());
}

TEST_F(PluginTest, InitReturnsZero) {
    auto fn = loader_->get_symbol<plugin_init_fn>("plugin_init");
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(), 0);
}

TEST_F(PluginTest, GetNameReturnsExpected) {
    auto fn = loader_->get_symbol<plugin_get_name_fn>("plugin_get_name");
    ASSERT_NE(fn, nullptr);
    EXPECT_STREQ(fn(), "challange_plugin");
}

TEST_F(PluginTest, AddReturnsCorrectSum) {
    auto fn = loader_->get_symbol<plugin_add_fn>("plugin_add");
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn(2, 3), 5);
    EXPECT_EQ(fn(-1, 1), 0);
    EXPECT_EQ(fn(0, 0), 0);
    EXPECT_EQ(fn(100, 200), 300);
}

TEST_F(PluginTest, AllSymbolsResolvable) {
    EXPECT_NE(loader_->get_symbol<plugin_init_fn>("plugin_init"), nullptr);
    EXPECT_NE(loader_->get_symbol<plugin_get_name_fn>("plugin_get_name"), nullptr);
    EXPECT_NE(loader_->get_symbol<plugin_add_fn>("plugin_add"), nullptr);
}

} // namespace
