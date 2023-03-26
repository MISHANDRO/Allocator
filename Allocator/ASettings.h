#pragma once

#include <vector>
#include <PoolSettings.h>

class ASettings {
public:
    ASettings() = delete;

    static void Init(size_t count_pools, const PoolSettings& default_pools,
                     const std::vector<PoolSettings>& another_pools = {}) {
        count_pools_ = count_pools;
        default_pools_ = default_pools;
        another_pools_ = another_pools;
    }

    static size_t CountPools()  {
        return count_pools_;
    }

    static PoolSettings DefaultPools() {
        return default_pools_;
    }

    static std::vector<PoolSettings> AnotherPools()  {
        return another_pools_;
    }

private:
    static size_t count_pools_;
    static PoolSettings default_pools_;
    static std::vector<PoolSettings> another_pools_;
};

size_t ASettings::count_pools_ = 4;
PoolSettings ASettings::default_pools_ = PoolSettings(8, 1024);
std::vector<PoolSettings> ASettings::another_pools_ = {PoolSettings(256, 32), PoolSettings(32, 256)};
