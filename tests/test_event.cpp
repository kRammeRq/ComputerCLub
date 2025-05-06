#include <gtest/gtest.h>
#include "Time.h"   
#include "ClubState.h"
#include <string>
#include <sstream>

// Тест конструктора и to_string
TEST(TimeTest, ConstructorAndToString) {
    Time t1;
    EXPECT_EQ(t1.to_string(), "00:00");

    Time t2(10, 30);
    EXPECT_EQ(t2.to_string(), "10:30");
}

// Тест from_string
TEST(TimeTest, FromString) {
    Time t1 = Time::from_string("10:30");
    EXPECT_EQ(t1.to_string(), "10:30");

    Time t2 = Time::from_string("01:45");
    EXPECT_EQ(t2.to_string(), "01:45");
}

// Тесты для операторов сравнения
TEST(TimeTest, ComparisonOperators) {
    Time t1(10, 30);
    Time t2(12, 45);
    Time t3(10, 30);

    // Проверяем операторы <, ==, <=
    EXPECT_TRUE(t1 < t2);
    EXPECT_FALSE(t2 < t1);
    EXPECT_TRUE(t1 == t3);
    EXPECT_FALSE(t1 == t2);
    EXPECT_TRUE(t1 <= t3);
    EXPECT_TRUE(t1 <= t2);
}

// Тест add_minutes
TEST(TimeTest, AddMinutes) {
    Time t1(10, 30);
    t1.add_minutes(90);  // 1.5 часа
    EXPECT_EQ(t1.to_string(), "12:00");

    t1.add_minutes(30);  // 30 минут
    EXPECT_EQ(t1.to_string(), "12:30");

    t1.add_minutes(60);  // 1 час
    EXPECT_EQ(t1.to_string(), "13:30");
}

// Тест оператора -
TEST(TimeTest, SubtractionOperator) {
    Time t1(10, 30);
    Time t2(12, 45);

    Time diff = t2 - t1;
    EXPECT_EQ(diff.to_string(), "02:15");  // Разница в 2 часа и 15 минут

    Time t3(10, 30);
    Time t4(10, 30);
    diff = t3 - t4;
    EXPECT_EQ(diff.to_string(), "00:00");  // Разница 0
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}