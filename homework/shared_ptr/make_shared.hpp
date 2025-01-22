#pragma once
#include <utility>
#include "shared_ptr.hpp"

namespace my {

template<typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    using CombinedBlock = typename shared_ptr<T>::ControlBlockWithObject;
    auto* combined = new CombinedBlock(std::forward<Args>(args)...);
    
    shared_ptr<T> result(&combined->object, &combined->control);
    result.control_block->deleter = [](T* ptr) {
        auto* combined_ptr = reinterpret_cast<CombinedBlock*>(
            reinterpret_cast<char*>(ptr) - offsetof(CombinedBlock, object)
        );
        delete combined_ptr;
    };
    
    return result;
}

} // namespace my