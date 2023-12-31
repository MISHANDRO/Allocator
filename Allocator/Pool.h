#pragma once

#include "PoolSettings.h"

#include <vector>

template<typename T>
class Pool {
public:

    explicit Pool(const PoolSettings& settings)
        : count_pieces_(settings.count_pieces)
        , size_pieces_(settings.size_pieces)
    {
        ptr_ = static_cast<T*>(operator new(size() * sizeof(T)));
        free.emplace_back(0, count_pieces_);
    }

    T* allocate(size_t n) {
        size_t allocate_count = (n % size_pieces_) ? n / size_pieces_ + 1 : n / size_pieces_;
        bool flag_find = false;
        size_t free_iterator = 0;

        for (auto& [index, count]: free) {
            if (count >= allocate_count) {
                flag_find = true;
                free_iterator = index;

                index += allocate_count;
                count -= allocate_count;

                break;
            }
        }

        if (flag_find) {
            return ptr_ + free_iterator * size_pieces_;
        } else {
            return nullptr;
        }
    }

    bool deallocate(T* ptr, size_t n) {
        if (ptr >= ptr_ && ptr <= (ptr_ + size())) {
            size_t index = ptr - ptr_;
            free.emplace_back(index, n);

            for (int j = 0; j < free.size(); j++) {
                auto cur = free[j];
                if (cur.first == index + n) {
                    free.back().second += cur.second;
                    free.erase(free.begin() + j);
                }

                if (cur.first + cur.second == index) {
                    free.back().first = free[j].first;
                    free.back().second += free[j].second;
                    free.erase(free.begin() + j);
                }
            }

            return true;
        } else {
            return false;
        }
    }

    [[nodiscard]] size_t size() const {
        return count_pieces_ * size_pieces_;
    }

    [[maybe_unused]] [[nodiscard]] size_t CountPieces() const {
        return count_pieces_;
    }

    [[nodiscard]] size_t SizePieces() const {
        return size_pieces_;
    }

    ~Pool() {
        operator delete(ptr_, size() * sizeof(T));
    }

private:
    std::vector<std::pair<size_t, size_t>> free;

    T* ptr_;

    size_t count_pieces_;
    size_t size_pieces_;
};
