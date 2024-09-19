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
    explicit shared_ptr(T* p = nullptr) : ptr(p), control_block(new ControlBlock()){}

    ~shared_ptr() {
        if (control_block)
        {
            if (--control_block->shared_count == 0)
            {
                if (ptr && control_block->deleter)
                {
                    control_block->deleter(ptr);
                }else{
                    delete ptr;
                }
                if (control_block->weak_count == 0)
                {
                    delete control_block;
                } 
            }
        }
    }


};
}  // namespace my