
# Build Sprint 05 — The Data Layer

## Container Choices

For Build Sprint 05, I updated Dungeon Delve to use STL containers based on how the game accesses its data.

### Party Roster — std::vector

I used `std::vector` for the party roster because it keeps elements together in memory, supports efficient iteration, and makes sorting straightforward.

The roster has three sorting views:
- Alphabetical order using a lambda comparator.
- Name length using a function object.
- Reverse alphabetical order using a function pointer.

Each view is a separate vector, so sorting does not modify the original roster.

### Bestiary — std::map

I added a `std::map<std::string, Monster*>` to index monsters by species.

The original bestiary vector continues to own the monster objects. The map stores non-owning pointers to those objects.

This allows the game to find monster prototypes by species without repeatedly searching through the entire vector.

### Encountered Species — std::set

I used `std::set<std::string>` to track monster species encountered during the campaign.

The set automatically removes duplicates, so encountering multiple goblins still results in only one "goblin" entry.

I also added a test that inserts repeated species names and verifies that only the unique names remain.

### Generic Containers

I implemented two class templates:

- `Registry<T>` stores values in a map using string keys.
- `Pool<T>` stores values in a vector and supports indexed access.

Both templates were instantiated and tested with `int` and `std::string`.

### LegacyLootBridge

The original raw C-style loot table remains unchanged.

I implemented `LegacyLootBridge` to transfer loot entries between the raw array and `Pool<std::string>`.

The `load()` operation copies loot names into the templated container. The `store()` operation exposes pointers to the strings already stored in the pool using `c_str()`.

Tests verify that the transferred names match, that the returned pointers refer to the pool's strings, and that the original loot table remains unchanged.

## Discussion 5 Benchmark Results

For Discussion 5, I benchmarked four STL containers using 100,000 elements.

Environment:
- MacBook Pro with Apple M4
- 16 GB RAM
- Apple Clang 16.0.0
- C++20
- Optimization: -O2
- Three benchmark runs, reporting median times

| Container | Insert (ms) | Find (ms) | Iterate (ms) |
|-----------|------------:|----------:|-------------:|
| vector | 0.299 | 2407.366 | 0.027 |
| list | 1.850 | 10623.679 | 0.210 |
| set | 18.637 | 11.306 | 2.045 |
| map | 20.343 | 12.771 | 1.930 |

## Performance Analysis

The vector had the fastest insertion and iteration times in my benchmark. Its iteration time was only 0.027 ms, compared with 0.210 ms for the list.

However, the vector's linear search took 2407.366 ms, while the map completed its search workload in 12.771 ms.

The list was the slowest for searching, taking 10623.679 ms. This surprised me because both vector and list use O(n) linear searches. It showed me that memory layout and cache locality can make a major difference even when the time complexity is the same.

The set and map had slower insertion times but much faster lookup times.

## Scaling Decision

If Dungeon Delve grows to include thousands of monsters, I would keep the vector for sequential storage and use the map for species-based lookup.

A linear search through a vector takes O(n) time, while std::map provides O(log n) lookup.

For the party roster, I would continue using a vector because party sizes are relatively small, and iteration and sorting are more important than keyed lookup.

For encountered species, std::set is appropriate because it automatically prevents duplicates and supports O(log n) insertion and lookup.

I would not replace these containers with std::list because the game does not benefit enough from frequent middle insertions or deletions to justify its slower iteration and searching.

## Conclusion

Build Sprint 05 helped me understand that choosing a container depends on how the program uses its data.

The benchmark results showed that vector is especially effective for sequential operations, while map and set are better suited to frequent keyed lookups.

By combining these containers, Dungeon Delve can keep its existing ownership structure while making monster lookup and encounter tracking more organized and scalable.
