#pragma once

#include <iostream>
namespace szl {
template <class T> class UniquePtr {
  public:
    UniquePtr() : m_data(nullptr) {}

    UniquePtr(T *_m_data) : m_data(_m_data) {}

    UniquePtr(const UniquePtr<T> &up) = delete;

    UniquePtr(UniquePtr<T> &&other) : m_data(other.release()) {}

    ~UniquePtr() {
        if (m_data != nullptr) {
            delete m_data;
            std::cout << "delete is called" << std::endl;
            m_data = nullptr;
        }
    }

    T *operator->() { return m_data; }

    T &operator*() { return *m_data; }

    T *get() { return m_data; }

    T *release() {
        auto data = m_data;
        m_data = nullptr;
        return data;
    }

    void reset(T *data = nullptr) {
        if (m_data != data) {
            delete m_data;
            m_data = data;
        }
    }

    UniquePtr &operator=(const UniquePtr<T> &other) = delete;

    UniquePtr &operator=(UniquePtr<T> &&other) {
        if (this == &other) {
            return *this;
        }
        reset(other.release());
        return *this;
    }

    void swap(UniquePtr<T> &other) {
        auto data = other.m_data;
        other.m_data = m_data;
        m_data = data;
    }

    T &operator[](int i) const { return m_data[i]; }

    explicit operator bool() const noexcept { return m_data != nullptr; }

  private:
    T *m_data;
};
} // namespace szl
