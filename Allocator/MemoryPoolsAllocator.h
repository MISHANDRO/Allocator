#pragma once

#include "ASettings.h"
#include "Pool.h"
#include "PoolComparator.h"

#include <stdexcept>
#include <algorithm>

template<typename T>
class MemoryPoolsAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;

    using reference = T&;
    using const_reference = const T&;

    using void_pointer = void*;
    using const_void_pointer = const void*;

    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    explicit MemoryPoolsAllocator() {
        pools_.resize(ASettings::CountPools());
        for (size_t i = 0; i < ASettings::CountPools(); i++) {
            if (i < ASettings::AnotherPools().size()) {
                pools_[i] = new Pool<value_type>(ASettings::AnotherPools()[i]);
            } else {
                pools_[i] = new Pool<value_type>(ASettings::DefaultPools());
            }
        }
    }

    MemoryPoolsAllocator(const MemoryPoolsAllocator& other) = default;
    MemoryPoolsAllocator& operator=(const MemoryPoolsAllocator& other) = default;

    pointer allocate(size_t n) {

        std::sort(pools_.begin(), pools_.end(), PoolComparator<T>(n));

        for (Pool<value_type>* i : pools_) {
            pointer ptr = i->allocate(n);

            if (ptr != nullptr) {
                return ptr;
            }
        }

        throw std::bad_alloc{};
    }

    void deallocate(pointer ptr, size_t n) {
        for (Pool<value_type>* i : pools_) {
            if (i->deallocate(ptr, n)) {
                return;
            }
        }
    }

    ~MemoryPoolsAllocator() {
        for (Pool<value_type>* i : pools_) {
            delete i;
        }
    }

private:
    std::vector<Pool<value_type>*> pools_;
};

template<typename T, typename U>
bool operator==(const MemoryPoolsAllocator<T>& lhs, const MemoryPoolsAllocator<U>& rhs) {
    return lhs.pools_ == rhs.pools_;
}

template<typename T, typename U>
bool operator!=(const MemoryPoolsAllocator<T>& lhs, const MemoryPoolsAllocator<U>& rhs) {
    return !(lhs == rhs);
}
