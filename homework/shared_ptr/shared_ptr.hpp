#pragma once
#include <atomic>

namespace my {

template <typename T>
class shared_ptr {
private:
    T* ptr;

    struct ControlBlock {
        std::atomic<size_t> shared_count;
        std::atomic<size_t> weak_count;
        void (*deleter)(T*);

        ControlBlock()
            : shared_count(1), weak_count(0), deleter(nullptr) {}
    };
    ControlBlock* control_block;

public:
    explicit shared_ptr(T* p = nullptr)
        : ptr(p), control_block(new ControlBlock()) {}

    shared_ptr(const shared_ptr& other)
        : ptr(other.ptr), control_block(other.control_block) {
        if (control_block) {
            ++control_block->shared_count;
        }
    }

    shared_ptr& operator=(const shared_ptr& otherSharedPtr) {
        shared_ptr(otherSharedPtr).swap(*this);
        return *this;
    }

    shared_ptr(shared_ptr&& otherSharedPtr) noexcept
        : ptr(otherSharedPtr.ptr), control_block(otherSharedPtr.control_block) {
        otherSharedPtr.ptr = nullptr;
        otherSharedPtr.control_block = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& otherSharedPtr) noexcept {
        shared_ptr(std::move(otherSharedPtr)).swap(*this);
        return *this;
    }

    ~shared_ptr() {
        if (control_block) {
            if (--control_block->shared_count == 0) {
                if (ptr && control_block->deleter) {
                    control_block->deleter(ptr);
                } else {
                    delete ptr;
                }
                if (control_block->weak_count == 0) {
                    delete control_block;
                }
            }
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    size_t use_count() const {
        return (control_block ? control_block->shared_count.load() : 0);
    }

    explicit operator bool() const { return ptr != nullptr; }

    void swap(shared_ptr& otherSharedPtr) noexcept {
        std::swap(ptr, otherSharedPtr.ptr);
        std::swap(control_block, otherSharedPtr.control_block);
    }

    void reset(T* p = nullptr) {
        if (p != ptr) {
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
    }
};
}  // namespace my