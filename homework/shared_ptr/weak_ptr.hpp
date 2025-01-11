#ifndef WEAK_PTR_HPP
#define WEAK_PTR_HPP

#include "shared_ptr.hpp"
#include <cstddef>

namespace my {

template <typename T>
class weak_ptr {
    friend class shared_ptr<T>;

public:
    weak_ptr() noexcept : ptr(nullptr), control_block(nullptr) {}
    
    weak_ptr(const shared_ptr<T>& sp) noexcept 
        : ptr(sp.ptr), control_block(sp.control_block) {
        if (control_block) {
            control_block->weak_count++;
        }
    }

    weak_ptr(const weak_ptr& other) noexcept 
        : ptr(other.ptr), control_block(other.control_block) {
        if (control_block) {
            control_block->weak_count++;
        }
    }

    weak_ptr(weak_ptr&& other) noexcept 
        : ptr(other.ptr), control_block(other.control_block) {
        other.ptr = nullptr;
        other.control_block = nullptr;
    }

    ~weak_ptr() {
        reset();
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            control_block = other.control_block;
            if (control_block) {
                control_block->weak_count++;
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

    void reset() noexcept {
        if (control_block) {
            --control_block->weak_count;
            if (control_block->shared_count == 0 && control_block->weak_count == 0) {
                delete control_block;
            }
        }
        ptr = nullptr;
        control_block = nullptr;
    }

    std::size_t use_count() const noexcept {
        return control_block ? control_block->shared_count.load() : 0;
    }

    bool expired() const noexcept {
        return use_count() == 0;
    }

    shared_ptr<T> lock() const noexcept {
        if (expired() || !control_block) {
            return shared_ptr<T>();
        }
        
        shared_ptr<T> sp;
        if (control_block->shared_count > 0) {
            sp.ptr = ptr;
            sp.control_block = control_block;
            ++control_block->shared_count;
        }
        return sp;
    }

private:
    T* ptr;
    typename shared_ptr<T>::ControlBlock* control_block;
};

} // namespace my

#endif // WEAK_PTR_HPP