#ifndef TEMPLATE_UTILS_H
#define TEMPLATE_UTILS_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <functional>

/**
 * ============================================================================
 * OOP CONCEPT: TEMPLATES (GENERIC PROGRAMMING)
 * Demonstrates:
 *  1. Function Templates:
 *     - genericSearch(): Searches any STL collection using a custom predicate.
 *     - printCollection(): Displays any collection using operator<<.
 *     - findById(): Generic lookup helper for entities with an ID getter.
 *  2. Class Template:
 *     - Repository<T>: A generic, type-safe repository managing entity collections.
 * ============================================================================
 */

// Function Template 1: Generic Search Utility
template <typename T, typename Predicate>
std::vector<T> genericSearch(const std::vector<T>& collection, Predicate predicate) {
    std::vector<T> results;
    for (const auto& item : collection) {
        if (predicate(item)) {
            results.push_back(item);
        }
    }
    return results;
}

// Function Template 2: Generic Display Utility
template <typename T>
void printCollection(const std::vector<T>& items, const std::string& heading = "") {
    if (!heading.empty()) {
        std::cout << "\n--- " << heading << " (Total: " << items.size() << ") ---\n";
    }
    if (items.empty()) {
        std::cout << "  [No records found]\n";
        return;
    }
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << " " << (i + 1) << ". " << *items[i] << "\n";
    }
}

// Function Template 3: Generic ID Lookup Utility
template <typename T, typename IdExtractor>
T findById(const std::vector<T>& collection, const std::string& targetId, IdExtractor extractor) {
    auto it = std::find_if(collection.begin(), collection.end(),
        [&](const T& item) {
            return extractor(item) == targetId;
        });
    if (it != collection.end()) {
        return *it;
    }
    return nullptr;
}

// Class Template: Generic Repository for Airline Entities
template <typename T>
class Repository {
private:
    std::vector<std::shared_ptr<T>> items;

public:
    Repository() = default;
    ~Repository() = default;

    void add(std::shared_ptr<T> item) {
        if (item) {
            items.push_back(item);
        }
    }

    bool remove(std::function<bool(const std::shared_ptr<T>&)> predicate) {
        auto it = std::remove_if(items.begin(), items.end(), predicate);
        if (it != items.end()) {
            items.erase(it, items.end());
            return true;
        }
        return false;
    }

    std::shared_ptr<T> find(std::function<bool(const std::shared_ptr<T>&)> predicate) const {
        for (const auto& item : items) {
            if (predicate(item)) {
                return item;
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<T>> filter(std::function<bool(const std::shared_ptr<T>&)> predicate) const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& item : items) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<T>>& getAll() {
        return items;
    }

    const std::vector<std::shared_ptr<T>>& getAll() const {
        return items;
    }

    size_t count() const {
        return items.size();
    }

    void clear() {
        items.clear();
    }

    void sort(std::function<bool(const std::shared_ptr<T>&, const std::shared_ptr<T>&)> comp) {
        std::sort(items.begin(), items.end(), comp);
    }
};

#endif // TEMPLATE_UTILS_H
