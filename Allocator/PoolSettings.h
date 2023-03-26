#pragma once

#include <cstdint>

struct PoolSettings {
    explicit PoolSettings(size_t count_pieces_, size_t size_pieces_)
        : count_pieces(count_pieces_)
        , size_pieces(size_pieces_)
    {}

    size_t count_pieces;
    size_t size_pieces;
};
