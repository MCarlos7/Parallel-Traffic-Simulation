#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <sstream>
#include <type_traits>

// Simple test framework
class TestRunner {
public:
    static TestRunner& getInstance() {
        static TestRunner instance;
        return instance;
    }

    int runTests() {
        int passed = 0;
        int failed = 0;
        for (const auto& test : tests_) {
            try {
                test.func();
                std::cout << "[PASS] " << test.name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << test.name << ": " << e.what() << std::endl;
                failed++;
            } catch (...) {
                std::cout << "[FAIL] " << test.name << ": unknown exception" << std::endl;
                failed++;
            }
        }
        std::cout << "\n====================\n";
        std::cout << "Results: " << passed << " passed, " << failed << " failed\n";
        std::cout << "====================\n";
        return failed;
    }

    struct TestInfo {
        std::string name;
        std::function<void()> func;
    };

    void registerTest(const std::string& name, std::function<void()> func) {
        tests_.push_back({name, func});
    }

private:
    std::vector<TestInfo> tests_;
};

// Helper to format values - uses int for enum classes
template<typename T>
std::string formatValue(const T& value) {
    if constexpr (std::is_enum_v<T>) {
        return std::to_string(static_cast<int>(value));
    } else {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }
}

#define TEST(name) void test_##name(); \
    struct TestRegistrar_##name { \
        TestRegistrar_##name() { \
            TestRunner::getInstance().registerTest(#name, test_##name); \
        } \
    } register_##name; \
    void test_##name()

#define EXPECT_TRUE(cond) if (!(cond)) throw std::runtime_error("EXPECT_TRUE failed: " #cond)
#define EXPECT_FALSE(cond) if (cond) throw std::runtime_error("EXPECT_FALSE failed: " #cond)
#define EXPECT_EQ(a, b) if ((a) != (b)) { \
    std::ostringstream ss; \
    ss << "EXPECT_EQ failed: " << #a << "==" << #b << " (left=" << formatValue(a) << " right=" << formatValue(b) << ")"; \
    throw std::runtime_error(ss.str()); \
}
#define EXPECT_NE(a, b) if ((a) == (b)) throw std::runtime_error("EXPECT_NE failed: " #a " != " #b)
#define EXPECT_LT(a, b) if (!((a) < (b))) throw std::runtime_error("EXPECT_LT failed: " #a " < " #b)
#define EXPECT_GT(a, b) if (!((a) > (b))) throw std::runtime_error("EXPECT_GT failed: " #a " > " #b)

#define ASSERT_TRUE(cond) if (!(cond)) throw std::runtime_error("ASSERT_TRUE failed: " #cond)
#define ASSERT_FALSE(cond) if (cond) throw std::runtime_error("ASSERT_FALSE failed: " #cond)
#define ASSERT_EQ(a, b) if ((a) != (b)) throw std::runtime_error("ASSERT_EQ failed: " #a " == " #b)

#define RUN_TESTS() int main() { return TestRunner::getInstance().runTests(); }
