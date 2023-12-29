#pragma once

#include <iostream>
namespace szl {
template <class T> class AutoPtr {
  public:
    AutoPtr() : m_data(nullptr) {}

    AutoPtr(T *_m_data) : m_data(_m_data) {}

    ~AutoPtr() {
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

    AutoPtr &operator=(AutoPtr<T> &other) {
        if (this == &other) {
            return *this;
        }
        m_data = other.release();
        return *this;
    }

  private:
    T *m_data;
};
} // namespace szl
