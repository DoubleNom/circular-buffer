#include <cstddef>
#include <vector>
#include <cstdio>

#ifdef CIRCULAR_BUFFER_TEST

#include <gtest/gtest_prod.h>

#endif

template<typename T>
class CircularBuffer {
public:
    CircularBuffer(size_t len) : m_external(false), m_capacity(len) {
        m_c = new T[len];
    }

    CircularBuffer(T *buff, size_t len)
            : m_c(buff), m_external(true), m_capacity(len) {}

    CircularBuffer(std::vector<T> vector) {
        m_c = vector.data();
        m_external = true;
        m_capacity = vector.size();
    }

    ~CircularBuffer() {
        if (!m_external)
            delete[] m_c;
    }

    T peek() {
        if (size() > 0) {
            return (*this)[0];
        } else {
            return T();
        }
    }

    size_t peek(T *buff, size_t len) {
        size_t count = 0;
        while (count < m_size) {
            buff[count] = (*this)[count];
            ++count;
        }
        return count;
    }

    T read() {
        if (m_size > 0) {
            size_t id = idx(m_read);
            incr(m_read);
            --m_size;
            return m_c[id];
        } else {
            return T();
        }
    }

    size_t read(T *buff, size_t len = 0) {
        size_t count = peek(buff, len);
        m_size -= count;
        incr(m_read, count);
        return count;
    }

    void pop_back(size_t len = 1) {
        if (len > m_size) len = m_size;
        incr(m_read, len);
        m_size -= len;
    }

    size_t write(const T t) {
        if (m_size < m_capacity) {
            m_c[m_write] = t;
            incr(m_write);
            ++m_size;
            return 1;
        } else {
            return 0;
        }
    }

    size_t write(const T *buff, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            // Early quit
            if (write(buff[i]) == 0) {
                return i;
            }
        }
        return len;
    }

    size_t write(std::vector<T> buff) {
        return write(buff.data(), buff.size());
    }

    size_t size() const { return m_size; }

    size_t length() const { return m_size; }

    size_t capacity() const { return m_capacity; }

    T operator[](size_t i) const { return m_c[idx(m_read, i)]; }

    T &operator[](size_t i) { return m_c[idx(m_read, i)]; }

private:
    T *m_c;
    bool m_external;
    size_t m_read = 0;
    size_t m_write = 0;
    size_t m_size = 0;
    size_t m_capacity = 0;

    size_t idx(size_t origin, size_t i = 0) {
        origin += i;
        while (origin >= m_capacity)
            origin -= m_capacity;
        return origin;
    }

    size_t incr(size_t &origin, size_t i = 1) {
        origin = idx(origin, i);
        return origin;
    }

#ifdef CIRCULAR_BUFFER_TEST

    FRIEND_TEST(CircularBuffer, PopBack);

    FRIEND_TEST(CircularBuffer, Write);

#endif
};
