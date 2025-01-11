#pragma once
#include <atomic>
#include "shared_ptr.hpp"

namespace my {

template <typename T>
class weak_ptr {
private:
    T* ptr{nullptr};
    typename shared_ptr<T>::ControlBlock* control_block{nullptr};

public:
    weak_ptr() noexcept = default;

    weak_ptr(const shared_ptr<T>& shared) noexcept
        : ptr(shared.ptr), control_block(shared.control_block) {
        if (control_block) {
            ++control_block->weak_count;
        }
    }

    weak_ptr(const weak_ptr& other) noexcept
        : ptr(other.ptr), control_block(other.control_block) {
        if (control_block) {
            ++control_block->weak_count;
        }
    }

    weak_ptr(weak_ptr&& other) noexcept
        : ptr(other.ptr), control_block(other.control_block) {
        other.ptr = nullptr;
        other.control_block = nullptr;
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            control_block = other.control_block;
            if (control_block) {
                ++control_block->weak_count;
            }
        }
        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            control_block = other.control_block;
            other.ptr = nullptr;
            other.control_block = nullptr;
        }
        return *this;
    }

    ~weak_ptr() {
        reset();
    }

    void reset() noexcept {
        if (control_block) {
            if (--control_block->weak_count == 0 && control_block->shared_count == 0) {
                delete control_block;
            }
            ptr = nullptr;
            control_block = nullptr;
        }
    }

    size_t use_count() const noexcept {
        return control_block ? control_block->shared_count.load() : 0;
    }

    bool expired() const noexcept {
        return use_count() == 0;
    }

    shared_ptr<T> lock() const noexcept {
        if (expired()) {
            return shared_ptr<T>();
        }

        shared_ptr<T> shared(ptr, control_block);
        if (control_block) {
            ++control_block->shared_count;
        }
        return shared;
    }
};

}  // namespace my