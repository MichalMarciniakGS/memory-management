#pragma once

namespace my {

template <typename T>
class unique_ptr {
public:
    unique_ptr(T* ptr = nullptr)
        : ptr_(ptr) {}

    ~unique_ptr() {
        delete ptr_;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& otherUnique_ptr)
        : ptr_(otherUnique_ptr.ptr_) {
        otherUnique_ptr.ptr_ = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& otherUnique_ptr) {
        if (this != &otherUnique_ptr) {
            delete ptr_;
            ptr_ = otherUnique_ptr.ptr_;
            otherUnique_ptr.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() {
        return *ptr_;
    }

    T* operator->() {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    T* release() noexcept {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) noexcept {
        if (ptr_ != new_ptr) {
            delete ptr_;
            ptr_ = new_ptr;
        }
    }

private:
    T* ptr_;
};

}  // namespace my