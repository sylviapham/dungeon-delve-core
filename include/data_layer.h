
#ifndef DATA_LAYER_H
#define DATA_LAYER_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

// Generic registry: stores values by unique string keys.
template <typename T>
class Registry {
private:
    std::map<std::string, T> entries_;

public:
    void add(const std::string& key, const T& value) {
        entries_[key] = value;
    }

    T* find(const std::string& key) {
        auto it = entries_.find(key);

        if (it == entries_.end()) {
            return nullptr;
        }

        return &it->second;
    }

    std::size_t size() const {
        return entries_.size();
    }
};

// Generic pool: stores values in insertion order.
template <typename T>
class Pool {
private:
    std::vector<T> items_;

public:
    void add(const T& item) {
        items_.push_back(item);
    }

    T* get(std::size_t index) {
        if (index >= items_.size()) {
            return nullptr;
        }

        return &items_[index];
    }

    const T* get(std::size_t index) const {
        if (index >= items_.size()) {
            return nullptr;
        }

        return &items_[index];
    }

    std::size_t size() const {
        return items_.size();
    }
};

// Adapter between legacy raw loot arrays and Pool<std::string>.
class LegacyLootBridge {
public:
    // Raw array -> templated pool
    static int load(const char* const* slots,
                    int count,
                    Pool<std::string>& out)
    {
        if (slots == nullptr || count < 0) {
            return -1;
        }

        // Validate everything before modifying the pool.
        for (int i = 0; i < count; ++i) {
            if (slots[i] == nullptr) {
                return -1;
            }
        }

        for (int i = 0; i < count; ++i) {
            out.add(slots[i]);
        }

        return count;
    }

    // Templated pool -> raw pointer array
    static int store(const Pool<std::string>& pool,
                     const char** dest,
                     int capacity)
    {
        if (dest == nullptr ||
            capacity < 0 ||
            static_cast<std::size_t>(capacity) < pool.size()) {
            return -1;
        }

        for (std::size_t i = 0; i < pool.size(); ++i) {
            dest[i] = pool.get(i)->c_str();
        }

        return static_cast<int>(pool.size());
    }
};

#endif // DATA_LAYER_H
