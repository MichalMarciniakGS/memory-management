#pragma once
#include <atomic>
#include <cstddef>
#include <utility>

namespace my {

template <typename T>
class weak_ptr;  // Forward declaration

template <typename T>
class shared_ptr {
    friend class weak_ptr<T>;

public:
    struct ControlBlock {
        std::atomic<std::size_t> shared_count;
        std::atomic<std::size_t> weak_count;
        
        ControlBlock() : shared_count(1), weak_count(0) {}
    };

    explicit shared_ptr(T* p = nullptr) 
        : ptr(p), control_block(new ControlBlock()) {}
    
    shared_ptr(const shared_ptr& other) 
        : ptr(other.ptr), control_block(other.control_block) {
        if (control_block) {
            ++control_block->shared_count;
        }
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            control_block = other.control_block;
            if (control_block) {
                ++control_block->shared_count;
            }
        }
        return *this;
    }

    shared_ptr(shared_ptr&& other) noexcept 
        : ptr(other.ptr), control_block(other.control_block) {
        other.ptr = nullptr;
        other.control_block = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            control_block = other.control_block;
            other.ptr = nullptr;
            other.control_block = nullptr;
        }
        return *this;
    }

    ~shared_ptr() {
        reset();
    }

    void swap(shared_ptr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(control_block, other.control_block);
    }

    T* get() const noexcept { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    
    std::size_t use_count() const noexcept {
        return control_block ? control_block->shared_count.load() : 0;
    }

    explicit operator bool() const noexcept { return ptr != nullptr; }

    void reset(T* p = nullptr) {
        if (control_block) {
            if (--control_block->shared_count == 0) {
                delete ptr;
                if (control_block->weak_count == 0) {
                    delete control_block;
                }
            }
        }
        ptr = p;
        if (p) {
            control_block = new ControlBlock();
        } else {
            control_block = nullptr;
        }
    }

private:
    T* ptr;
    ControlBlock* control_block;
};

} // namespace my