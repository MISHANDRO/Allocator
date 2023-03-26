#pragma once

#include "ASettings.h"
#include "Pool.h"

#include <stdexcept>
#include <algorithm>

template<typename T>
class Comparator {
public:
    explicit Comparator(size_t need_size)
            : need_size_(need_size)
    {}

    bool operator()(const Pool<T>* lhr, const Pool<T>* rhr)
    {
        if (lhr->size() < need_size_) {
            return false;
        }

        if (rhr->size() < need_size_) {
            return true;
        }

        size_t excess1 = lhr->SizePieces() - (need_size_ % lhr->SizePieces());
        size_t excess2 = rhr->SizePieces() - (need_size_ % rhr->SizePieces());

        if (excess1 == excess2) {
            return lhr->SizePieces() > rhr->SizePieces();
        } else {
            return excess1 < excess2;
        }
    }

private:
    size_t need_size_;
};

template<typename T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };

    explicit Allocator() {
        pools_.resize(ASettings::CountPools());
        for (size_t i = 0; i < ASettings::CountPools(); i++) {
            if (i < ASettings::AnotherPools().size()) {
                pools_[i] = new Pool<value_type>(ASettings::AnotherPools()[i]);
            } else {
                pools_[i] = new Pool<value_type>(ASettings::DefaultPools());
            }
        }
    }

    Allocator(const Allocator& other) = default;
    Allocator& operator=(const Allocator& other) = default;

    pointer allocate(size_t n) {

        std::sort(pools_.begin(), pools_.end(), Comparator<T>(n));

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

    ~Allocator() {
        for (Pool<value_type>* i : pools_) {
            delete i;
        }
    }

private:
    std::vector<Pool<value_type>*> pools_;
};

template<typename T, typename U>
bool operator==(const Allocator<T>& lhr, const Allocator<U>& rhl) {
    return false;
}

template<typename T, typename U>
bool operator!=(const Allocator<T>& lhr, const Allocator<U>& rhr) {
    return !(lhr == rhr);
}
