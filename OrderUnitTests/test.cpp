#include <gtest/gtest.h>
#include "order.h"

TEST(MoneyTest, Constructor_PositiveValue) {
    Money m(500);
    EXPECT_EQ(m.cents, 500);
}

TEST(MoneyTest, OperatorPlus) {
    Money m1(100);
    Money m2(50);
    EXPECT_EQ((m1 + m2).cents, 150);
}

TEST(MoneyTest, OperatorPlusEqual) {
    Money m1(100);
    Money m2(50);
    m1 += m2;
    EXPECT_EQ(m1.cents, 150);
}

TEST(MoneyTest, OperatorEquality) {
    Money m1(200);
    Money m2(200);
    EXPECT_TRUE(m1 == m2);
}

TEST(DiscountTest, ZeroPercent) {
    Money original(1000);
    EXPECT_EQ(apply_discount(original, 0).cents, 1000);
}

TEST(DiscountTest, HundredPercent) {
    Money original(1000);
    EXPECT_EQ(apply_discount(original, 100).cents, 0);
}

class OrderTest : public ::testing::Test {
protected:
    Order order;
    OrderItem createItem(std::string sku, long long price, int qty) {
        return OrderItem{ sku, Money(price), qty };
    }
};

TEST_F(OrderTest, AddItem_IncreasesCount) {
    order.add_item(createItem("A1", 100, 1));
    EXPECT_EQ(order.items_count(), 1);
}

TEST_F(OrderTest, Subtotal_MultipleItems) {
    order.add_item(createItem("X1", 100, 2));
    order.add_item(createItem("X2", 50, 3));
    EXPECT_EQ(order.subtotal().cents, 350);
}

TEST_F(OrderTest, TotalWithDiscountAndVat) {
    order.add_item(createItem("X1", 10000, 1));
    Money result = order.total_with_discount_and_vat(10, 20);
    EXPECT_EQ(result.cents, 10800);
}

TEST_F(OrderTest, IsValid_Checks) {
    EXPECT_FALSE(order.is_valid());
    order.add_item(createItem("SKU", 100, 1));
    EXPECT_TRUE(order.is_valid());
}