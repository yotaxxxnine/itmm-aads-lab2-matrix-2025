#include "tmatrix.h" 
#include <gtest.h> 

TEST(TDynamicVector, can_create_vector_with_positive_length) {
    ASSERT_NO_THROW(TDynamicVector<int> v(5));
}

TEST(TDynamicVector, cant_create_too_large_vector) {
    ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TDynamicVector, throws_when_create_vector_with_negative_length) {
    ASSERT_ANY_THROW(TDynamicVector<int> v(-5));
}

TEST(TDynamicVector, can_create_copied_vector) {
    TDynamicVector<int> v(10);
    ASSERT_NO_THROW(TDynamicVector<int> v1(v));
}

TEST(TDynamicVector, copied_vector_is_equal_to_source_one) {
    TDynamicVector<int> v(5);
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] = static_cast<int>(i);
    }
    TDynamicVector<int> v1(v);
    EXPECT_EQ(v, v1);
}

TEST(TDynamicVector, copied_vector_has_its_own_memory) {
    TDynamicVector<int> v(3);
    v[0] = 10;
    TDynamicVector<int> v1(v);
    v1[0] = 20;
    EXPECT_NE(v[0], v1[0]);
}

TEST(TDynamicVector, can_get_size) {
    TDynamicVector<int> v(4);
    EXPECT_EQ(4, v.size());
}

TEST(TDynamicVector, throws_when_set_element_with_negative_index) {
    TDynamicVector<int> v(3);
    ASSERT_ANY_THROW(v.at(-1) = 10);
}

TEST(TDynamicVector, throws_when_set_element_with_too_large_index) {
    TDynamicVector<int> v(3);
    ASSERT_ANY_THROW(v.at(3) = 50);
}

TEST(TDynamicVector, can_assign_vector_to_itself) {
    TDynamicVector<int> v(3);
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] = static_cast<int>(i);
    }
    ASSERT_NO_THROW(v = v);
    EXPECT_EQ(v, v);
}

TEST(TDynamicVector, can_assign_vectors_of_equal_size) {
    TDynamicVector<int> v1(5);
    TDynamicVector<int> v2(5);
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] = static_cast<int>(i);
    }
    ASSERT_NO_THROW(v2 = v1);
    EXPECT_EQ(v1, v2);
}

TEST(TDynamicVector, assign_operator_change_vector_size) {
    TDynamicVector<int> v1(5);
    TDynamicVector<int> v2(10);
    v2 = v1;
    EXPECT_EQ(v1.size(), v2.size());
}

TEST(TDynamicVector, can_assign_vectors_of_different_size) {
    TDynamicVector<int> v1(5);
    TDynamicVector<int> v2(10);
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] = static_cast<int>(i);
    }
    ASSERT_NO_THROW(v2 = v1);
    EXPECT_EQ(v1, v2);
}

TEST(TDynamicVector, compare_equal_vectors_return_true) {
    TDynamicVector<int> v1(5), v2(5);
    for (size_t i = 0; i < v1.size(); ++i)
        v1[i] = v2[i] = static_cast<int>(i);
    EXPECT_TRUE(v1 == v2);
}

TEST(TDynamicVector, compare_vector_with_itself_return_true) {
    TDynamicVector<int> v(5);
    EXPECT_TRUE(v == v);
}

TEST(TDynamicVector, vectors_with_different_size_are_not_equal) {
    TDynamicVector<int> v(5), v2(10);
    EXPECT_FALSE(v == v2);
}

TEST(TDynamicVector, can_add_scalar_to_vector) {
    TDynamicVector<int> v(3);
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = static_cast<int>(i);
    TDynamicVector<int> res = v + 5;
    EXPECT_EQ(5, res[0]);
    EXPECT_EQ(6, res[1]);
    EXPECT_EQ(7, res[2]);
}

TEST(TDynamicVector, can_subtract_scalar_from_vector) {
    TDynamicVector<int> v(3);
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = static_cast<int>(i);
    TDynamicVector<int> res = v - 1;
    EXPECT_EQ(-1, res[0]);
    EXPECT_EQ(0, res[1]);
    EXPECT_EQ(1, res[2]);
}

TEST(TDynamicVector, can_multiply_scalar_by_vector) {
    TDynamicVector<int> v(3);
    for (size_t i = 0; i < v.size(); ++i)
        v[i] = static_cast<int>(i);
    TDynamicVector<int> res = v * 2;
    EXPECT_EQ(0, res[0]);
    EXPECT_EQ(2, res[1]);
    EXPECT_EQ(4, res[2]);
}

TEST(TDynamicVector, can_add_vectors_with_equal_size) {
    TDynamicVector<int> v1(3), v2(3);
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] = static_cast<int>(i);
        v2[i] = static_cast<int>(i * 2);
    }
    TDynamicVector<int> res = v1 + v2;
    EXPECT_EQ(0, res[0]);
    EXPECT_EQ(3, res[1]);
    EXPECT_EQ(6, res[2]);
}

TEST(TDynamicVector, cant_add_vectors_with_not_equal_size) {
    TDynamicVector<int> v1(3), v2(5);
    ASSERT_ANY_THROW(v1 + v2);
}
TEST(TDynamicVector, can_subtract_vectors_with_equal_size) {
    TDynamicVector<int> v1(3), v2(3);
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] = static_cast<int>(i * 3);
        v2[i] = static_cast<int>(i);
    }
    TDynamicVector<int> res = v1 - v2;
    EXPECT_EQ(0, res[0]);
    EXPECT_EQ(2, res[1]);
    EXPECT_EQ(4, res[2]);
}

TEST(TDynamicVector, cant_subtract_vectors_with_not_equal_size) {
    TDynamicVector<int> v1(3), v2(5);
    ASSERT_ANY_THROW(v1 - v2);
}

TEST(TDynamicVector, can_multiply_vectors_with_equal_size) {
    TDynamicVector<int> v1(3), v2(3);
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] = static_cast<int>(i + 1);
        v2[i] = static_cast<int>(i + 2);
    }
    int res = v1 * v2;
    EXPECT_EQ(20, res);
}

TEST(TDynamicVector, cant_multiply_vectors_with_not_equal_size) {
    TDynamicVector<int> v1(3), v2(5);
    ASSERT_ANY_THROW(v1 * v2);
}