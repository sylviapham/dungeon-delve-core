
#include <cstdio>
#include <set>
#include <string>

#include "data_layer.h"
#include "dungeon_delve.h"

int main()
{
    // TEST 1: Registry<T> with two types
    Registry<int> number_registry;
    number_registry.add("level", 5);

    Registry<std::string> name_registry;
    name_registry.add("hero", "Ash");

    if (number_registry.find("level") == nullptr ||
        *number_registry.find("level") != 5 ||
        name_registry.find("hero") == nullptr ||
        *name_registry.find("hero") != "Ash") {
        std::printf("FAILED: Registry tests\n");
        return 1;
    }

    std::printf("PASSED: Registry<T>\n");

    // TEST 2: Pool<T> with two types
    Pool<int> number_pool;
    number_pool.add(10);
    number_pool.add(20);

    Pool<std::string> name_pool;
    name_pool.add("Ash");
    name_pool.add("Briar");

    if (number_pool.size() != 2 ||
        name_pool.size() != 2 ||
        *number_pool.get(0) != 10 ||
        *name_pool.get(1) != "Briar") {
        std::printf("FAILED: Pool tests\n");
        return 1;
    }

    std::printf("PASSED: Pool<T>\n");

    // TEST 3: std::set removes duplicates
    std::set<std::string> encountered_species;

    encountered_species.insert("goblin");
    encountered_species.insert("kobold");
    encountered_species.insert("goblin");
    encountered_species.insert("bat");
    encountered_species.insert("kobold");

    if (encountered_species.size() != 3) {
        std::printf("FAILED: duplicate species\n");
        return 1;
    }

    std::printf("PASSED: unique species\n");

    // TEST 4: LegacyLootBridge with the real loot table
    Pool<std::string> loot_pool;

    const int count = loot_count();

    if (count != 12) {
        std::printf("FAILED: unexpected loot count\n");
        return 1;
    }

    const char* original[12];

    for (int i = 0; i < count; ++i) {
        original[i] = loot_at(i);
    }

    if (LegacyLootBridge::load(original, count, loot_pool)
            != count) {
        std::printf("FAILED: bridge load\n");
        return 1;
    }

    const char* restored[12] = {};

    if (LegacyLootBridge::store(loot_pool, restored, 12)
            != count) {
        std::printf("FAILED: bridge store\n");
        return 1;
    }

    for (int i = 0; i < count; ++i) {
        if (loot_pool.get(i) == nullptr ||
            *loot_pool.get(i) != original[i] ||
            restored[i] != loot_pool.get(i)->c_str()) {
            std::printf("FAILED: bridge data mismatch\n");
            return 1;
        }

        // Confirm the original raw table is unchanged.
        if (loot_at(i) != original[i]) {
            std::printf("FAILED: raw loot table modified\n");
            return 1;
        }
    }

    std::printf("PASSED: LegacyLootBridge\n");

    std::printf("\nALL BUILD 05 TESTS PASSED\n");
    return 0;
}
