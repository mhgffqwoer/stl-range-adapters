#include <gtest/gtest.h>

#include <sstream>

#include "Ranges.hpp"

// Verify that chaining two transform adapters works correctly. The first
// square each element, the second adds 5. Output is concatenated into a string.
TEST(AdaptersTest, TransformTest) {
    std::stringstream out;
    std::vector<int32_t> myvec = {1, 2, 3, 4, 5};
    for (auto i : myvec | transform([](int32_t j) { return j * j; }) |
                      transform([](int32_t x) { return x + 5; })) {
        out << i;
    }

    ASSERT_EQ(out.str(), "69142130");
}

// Apply a transform to square the values, then filter out numbers <= 2.
// The remaining values are written to stream and compared.
TEST(AdaptersTest, FilterTest) {
    std::stringstream out;
    std::vector<int32_t> myvec = {1, 2, 3, 4, 5};
    for (auto i : myvec | transform([](int32_t j) { return j * j; }) |
                      filter([](int32_t x) { return x > 2; })) {
        out << i;
    }

    ASSERT_EQ(out.str(), "491625");
}

// Ensure take(N) limits the number of elements after a transformation.
TEST(AdaptersTest, TakeTest) {
    std::stringstream out;
    std::vector<int32_t> myvec = {1, 2, 3, 4, 5};
    for (auto i :
         myvec | transform([](int32_t j) { return j * j; }) | take(4)) {
        out << i;
    }

    ASSERT_EQ(out.str(), "14916");
}

// Verify drop(N) skips the first N elements from the transformed range.
TEST(AdaptersTest, DropTest) {
    std::stringstream out;
    std::vector<int32_t> myvec = {1, 2, 3, 4, 5};
    for (auto i :
         myvec | transform([](int32_t j) { return j * j; }) | drop(3)) {
        out << i;
    }

    ASSERT_EQ(out.str(), "1625");
}

// Ensure reverse() iterates over the squared values in reverse order.
TEST(AdaptersTest, ReverseTest) {
    std::stringstream out;
    std::vector<int32_t> myvec = {1, 2, 3, 4, 5};
    for (auto i : myvec | transform([](int32_t j) { return j * j; }) | reverse()) {
        out << i;
    }

    ASSERT_EQ(out.str(), "2516941");
}

// Combination of keys(), transform and reverse over a map. Outputs squared
// keys in reverse order.
TEST(AdaptersTest, KeysTest) {
    std::stringstream out;
    std::map<int32_t, int32_t> mymap = {{1, 90}, {2, 80}, {3, 70}, {4, 60}};

    for (auto i : mymap | keys() | transform([](int32_t j) { return j * j; }) | reverse()) {
        out << i;
    }

    ASSERT_EQ(out.str(), "16941");
}

TEST(AdaptersTest, ValuesTest) {
    std::stringstream out;
    std::map<int32_t, int32_t> mymap = {{1, 90}, {2, 80}, {3, 70}, {4, 60}};

    for (auto i : mymap | values() |
                      transform([](int32_t j) { return j * j; }) | reverse()) {
        out << i;
    }

    ASSERT_EQ(out.str(), "3600490064008100");
}