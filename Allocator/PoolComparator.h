#pragma once

#include "Pool.h"

template<typename T>
class PoolComparator {
public:
    explicit PoolComparator(size_t need_size)
        : need_size_(need_size)
    {}

    bool operator()(const Pool<T>* lhs, const Pool<T>* rhs)
    {
        if (lhs->size() < need_size_) {
            return false;
        }

        if (rhs->size() < need_size_) {
            return true;
        }

        size_t excess1 = lhs->SizePieces() - (need_size_ % lhs->SizePieces());
        size_t excess2 = rhs->SizePieces() - (need_size_ % rhs->SizePieces());

        if (excess1 == excess2) {
            return lhs->SizePieces() > rhs->SizePieces();
        } else {
            return excess1 < excess2;
        }
    }

private:
    size_t need_size_;
};
