#pragma once

namespace szl {
template <class T> class SharedPtr {
  public:
    SharedPtr() : m_data(nullptr), m_count(0) {}
    SharedPtr(T *data) : m_data(data) {
        if (data != nullptr) {
            m_count = new int(1);
        }
    }
    SharedPtr(const SharedPtr<T> &other)
        : m_data(other.m_data), m_count(other.m_count) {
        if (m_data != nullptr) {
            (*m_count)++;
        }
    }
    SharedPtr(const SharedPtr<T> &&other) noexcept
        : m_data(other.m_data), m_count(other.m_count) {
        other.m_data = nullptr;
        other.m_count = nullptr;
    }
    ~SharedPtr() {
        if (m_data != nullptr) {
            (*m_count)--;
            if (*m_count <= 0) {
                delete m_data;
                m_data = nullptr;
                delete m_count;
                m_count = nullptr;
            }
        }
    }
    // get
    void reset(T *data = nullptr) {
        if (m_data == data) {
            return;
        }
        if (m_data == nullptr) {
            if (data != nullptr) {
                m_data = data;
                m_count = new int(1);
            }
            return;
        }
        (*m_count)--;
        if (*m_count <= 0) {
            delete m_data;
            m_data = nullptr;
            delete m_count;
            m_count = nullptr;
        }
        m_data = data;
        if (data != nullptr) {
            m_count = new int(1);
        }
    }

    int use_count() const {
        if (m_data == nullptr) {
            return 0;
        }
        return *m_count;
    }

    bool unique() const {
        if (m_data == nullptr) {
            return false;
        }
        return *m_count == 1;
    }

    // swap
    // ->
    // *
    // bool
    // = &
    // = &&

  private:
    T *m_data;
    int *m_count;
};

} // namespace szl
