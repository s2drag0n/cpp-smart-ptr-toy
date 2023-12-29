# smart-ptr

## RAII

## auto_ptr

方法

- ->
- \*
- get
- release
- reset
- =
```cpp
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
```
auto_ptr 的问题是：一个对象的拥有权可以在多个auto_ptr之间进行转移，这导致对象的拥有权非常混乱，有时很难找到是哪一个auto_ptr拥有当前当前对象。

## unique_ptr

unique_ptr的拷贝构造函数被delete，但是允许移动构造函数，赋值运算符同样被delete，但是允许移动赋值。
```cpp
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
```
## shared_ptr

没有release方法，因为需要多个指针共享同一个对象的拥有权。

```cpp
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
```
### shared_ptr存在的循环引用问题

类A中有一个指向类B的智能指针，B类中存在一个指向A类的智能指针。这样的话，释放A的前提是先释放B，因为B中存在对A的引用计数。反之亦然，所以形成了释放死锁。

## weak_ptr

shared_ptr存在循环引用的问题，所以引入了weak_ptr。而weak_ptr不会增加引用计数，所以只需在上面的情境中将A或者B的指针设置为weak_ptr，就不会出现循环引用导致的问题。

其次就是无法直接通过weak_ptr去访问对象，而是需要通过`lock`函数来返回一个临时的shared_ptr来进行访问。在调用`lock`之前最好先调用`expired`来判断是否过期。

weak_ptr的主要成员方法和shared_ptr区别不大。


