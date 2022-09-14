#include <gtest/gtest.h>
#include "circular_buffer.hpp"

TEST(CircularBuffer, Constructor_Size) {
    CircularBuffer<int> cb(5);
    ASSERT_EQ(cb.capacity(), 5);
}

TEST(CircularBuffer, Constructor_Array) {
    int buff[5] = {};
    CircularBuffer<int> cb(buff, 5);
    ASSERT_EQ(cb.capacity(), 5);

    cb.write({0, 1, 2, 3, 4});
    ASSERT_EQ(cb[0], 0);
    ASSERT_EQ(cb[0], buff[0]);
    ASSERT_EQ(cb[4], 4);
    ASSERT_EQ(cb[4], buff[4]);
}

TEST(CircularBuffer, Constructor_VectorBuffer) {
    std::vector<int> buff(5);
    CircularBuffer<int> cb(buff.data(), 5);
    ASSERT_EQ(cb.capacity(), 5);

    cb.write({0, 1, 2, 3, 4});
    ASSERT_EQ(cb[0], 0);
    ASSERT_EQ(cb[0], buff[0]);
    ASSERT_EQ(cb[4], 4);
    ASSERT_EQ(cb[4], buff[4]);
}

TEST(CircularBuffer, Constructor_Vector) {
    std::vector<int> vector(5);
    CircularBuffer<int> cb(vector);
    ASSERT_EQ(cb.capacity(), 5);
    ASSERT_EQ(cb.size(), 0);
    cb.write({0, 1, 2, 3, 4});
    ASSERT_EQ(cb.size(), 5);
    ASSERT_EQ(cb[0], 0);
    ASSERT_EQ(vector[0], 0);
    ASSERT_EQ(cb[4], 4);
    ASSERT_EQ(vector[4], 4);
}

TEST(CircularBuffer, Peek) {
    CircularBuffer<int> cb(5);
    cb.write(1);
    ASSERT_EQ(cb.size(), 1);
    ASSERT_EQ(cb.peek(), 1);
    ASSERT_EQ(cb.size(), 1);
    ASSERT_EQ(cb.peek(), 1);

    int buff[10];
    cb.write({2, 3});
    ASSERT_EQ(cb.size(), 3);
    ASSERT_EQ(cb.peek(buff, 3), 3);
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 2);
    ASSERT_EQ(buff[2], 3);

    ASSERT_EQ(cb.peek(buff, 10), 3);
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 2);
    ASSERT_EQ(buff[2], 3);

    cb.pop_back(cb.size());
    ASSERT_EQ(cb.size(), 0);
    cb.write({4, 5, 6, 7, 8});
    ASSERT_EQ(cb.peek(), 4);
    ASSERT_EQ(cb.peek(buff, 10), 5);
    for (int i = 0; i < 5; ++i) ASSERT_EQ(buff[i], i + 4);
}

TEST(CircularBuffer, Read) {
    CircularBuffer<int> cb(5);
    cb.write(1);
    ASSERT_EQ(cb.size(), 1);
    ASSERT_EQ(cb.read(), 1);
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.read(), 0);

    int buff[10];
    cb.write({1, 2, 3});
    ASSERT_EQ(cb.size(), 3);
    ASSERT_EQ(cb.read(buff, 3), 3);
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 2);
    ASSERT_EQ(buff[2], 3);
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.read(buff, 3), 0);

    cb.write({4, 5, 6, 7, 8});
    ASSERT_EQ(cb.read(buff, 10), 5);
    for (int i = 0; i < 5; ++i) ASSERT_EQ(buff[i], i + 4);
}

TEST(CircularBuffer, PopBack) {
    CircularBuffer<int> cb(10);
    ASSERT_EQ(cb.size(), 0);
    cb.write({1, 2, 3});
    ASSERT_EQ(cb.size(), 3);
    ASSERT_EQ(cb.m_read, 0);
    cb.pop_back();
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb.m_read, 1);
    cb.pop_back(2);
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.m_read, 3);
    cb.pop_back();
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.m_read, 3);
    cb.write({1, 2, 3, 4, 5});
    ASSERT_EQ(cb.size(), 5);
    cb.pop_back(6);
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.m_read, 8);
}

TEST(CircularBuffer, Write) {
    int buff[5];
    CircularBuffer<int> cb(buff, 5);
    ASSERT_EQ(cb.size(), 0);
    ASSERT_EQ(cb.write(0), 1);
    ASSERT_EQ(cb.m_write, 1);
    ASSERT_EQ(buff[0], 0);
    ASSERT_EQ(cb.size(), 1);
    ASSERT_EQ(cb.write({1, 2, 3, 4}), 4);
    ASSERT_EQ(cb.size(), 5);
    ASSERT_EQ(cb.m_write, 0);
    for (int i = 0; i < 5; ++i) ASSERT_EQ(buff[i], i);
    ASSERT_EQ(cb.write(1), 0);
    ASSERT_EQ(cb.size(), 5);
    ASSERT_EQ(cb.m_write, 0);
    cb.pop_back(cb.size());
    ASSERT_EQ(cb.write({1, 2, 3, 4, 5, 6}), 5);
}