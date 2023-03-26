#pragma once

#include "PoolSettings.h"

#include <vector>
#include <cmath>
#include <cstdint>
#include <map>

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
        bool flag = false;
        size_t free_iterator = 0;

        for (int j = 0; j < free.size(); j++) {
            auto i = free[j];
            if (i.second >= allocate_count) {
                flag = true;
                free_iterator = i.first;
                if (i.second == allocate_count) {
                    free.erase(free.begin() + j);
                } else {
                    free[j].first += allocate_count;
                    free[j].second -= allocate_count;
                }
                break;
            }
        }

        if (flag) {
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
                auto i = free[j];
                if (i.first == index + n) {
                    free.back().second += i.second;
                    free.erase(free.begin() + j);
                }

                if (i.first + i.second == index) {
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

    [[nodiscard]] size_t CountPieces() const {
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
