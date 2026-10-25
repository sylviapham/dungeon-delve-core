# TRIAGE.md

## 1. The three defects

### Defect 1 — Monster Iterator Invalidation

**How I found it:**
I ran the regression tests with AddressSanitizer enabled. The program reported a heap-use-after-free in `run_monster_round()` around line 439 of `dungeon_delve.cpp`. I examined the encounter loop and the monster summoning code to figure out what was causing it.

**What was actually wrong:**
The encounter loop used a vector iterator to go through the monsters. When a monster used its summon ability, `resolve_ability()` added another monster to the same vector using `wave.push_back()`. This could cause the vector to reallocate its storage, making the original iterator invalid. The program then tried to continue using that iterator, which caused the memory error.

**What I changed:**
I replaced the iterator-based loop with an index-based loop using `size_t i`. This allows the program to access monsters by their current position in the vector without relying on an iterator that could become invalid after reallocation. The `roster_survives_reallocation` regression test passed after the change.

### Defect 2 — Loot Table Buffer Overflow

**How I found it:**
After fixing the monster loop, I ran the regression tests again. AddressSanitizer reported a global-buffer-overflow in the loot selection code around line 185 of `dungeon_delve.cpp`.

**What was actually wrong:**
The program used `dd_rand_range(0, LOOT_COUNT)` to choose an item from the loot table. Since the random function includes the upper limit, it could generate an index equal to `LOOT_COUNT`. However, array indexes start at zero, so the last valid index is `LOOT_COUNT - 1`. This allowed the program to access memory outside the loot table.

**What I changed:**
I changed the random selection to `dd_rand_range(0, LOOT_COUNT - 1)`. This keeps the selected index within the valid array boundaries. The `loot_table_intact` regression test passed afterward.

### Defect 3 — Armor Applied Twice

**How I found it:**
The `encounter_resolves` regression test was still failing after the other two defects were fixed. The test showed that an attack dealing 19 damage without armor only removed 13 HP when the monster had 3 armor. The expected result was 16 HP removed.

**What was actually wrong:**
The attack calculation was reducing damage by armor before passing it to `apply_damage_to_monster()`. That function also applied armor, so the armor value was subtracted twice. This caused monsters to receive less damage than intended.

**What I changed:**
I changed `resolve_attack_on_monster()` to pass `raw_damage` into `apply_damage_to_monster()` instead of `after_armor`. This allows armor to be applied only once. After rebuilding the project, all three regression tests passed.

## 2. Ownership Table

The original program used raw pointers to manage dynamically allocated heroes, monsters, and merchants. I converted the owning pointer aliases to `std::unique_ptr` and replaced the manual allocations with `std::make_unique`.

| Allocation site | Who owned it before | Converted to | Why that choice |
|:--|:--|:--|:--|
| `make_hero()` — line 119 | The raw `HeroPtr` returned to the caller, including `hero_owner` in `run_campaign()` | `std::unique_ptr<Hero>` | The hero has one owner, and automatic cleanup prevents memory leaks. |
| `make_monster()` — line 137 | Raw `MonsterPtr` objects stored in `Bestiary` or encounter `Wave` containers | `std::unique_ptr<Monster>` | Each monster has one owner, and the containers automatically clean up their monsters. |
| `make_merchant()` — line 155 | The raw `MerchantPtr` stored in `yarrow_owner` | `std::unique_ptr<Merchant>` | The merchant has one owner and no longer requires manual memory management. |
| `load_hero()` — line 615 | A raw `HeroPtr` returned to the caller, with manual deletion on a failed load | `std::unique_ptr<Hero>` | The hero is automatically cleaned up if loading fails or when its owner is destroyed. |

### Non-owning pointers and references

The program still uses raw pointers and references when a function only needs to access an object without owning it.

In `run_campaign()`, `Hero* hero` and `Merchant* yarrow` are non-owning aliases. Their objects are owned by `hero_owner` and `yarrow_owner`.

The `raw()` helper in `dungeon_delve.h` returns a non-owning `Monster*` from a smart pointer. It uses `.get()` to access the object without transferring ownership.

Functions such as `resolve_ability()` and `run_monster_round()` also access monsters without taking ownership. The owning `Wave` container remains responsible for their lifetimes.

Readers can identify these non-owning uses through the raw-pointer or reference types, the ownership comments, and the fact that these functions do not delete the objects.

## 3. The Three Worst Things About This Codebase

1. **Unsafe iteration during monster summoning — lines 316–320 and 438–439.**
   The encounter loop originally relied on iterators even though monsters could be added to the vector during combat. This caused iterator invalidation and a use-after-free error. I fixed the immediate issue by switching to an index-based loop. In a future build, I would consider separating newly summoned monsters from the current encounter iteration to make the behavior easier to understand.

2. **Manual memory ownership — lines 119, 137, 155, and 615–618 in the original code.**
   The program originally used `new` and raw pointers to create objects. This made ownership harder to track and created opportunities for memory leaks or incorrect deletion. I converted these allocations to `std::make_unique` and used `std::unique_ptr` to manage their lifetimes. I would continue improving the ownership documentation in later builds.

3. **Magic numbers and ability tags — around lines 90–100 in `dungeon_delve.h` and the `resolve_ability()` function.**
   The program uses integer constants to represent different monster abilities. This makes the code harder to understand and maintain because the meaning of each number depends on definitions elsewhere. In a later build, I would consider replacing these constants with an enum and organizing the ability logic more clearly. I am leaving that redesign for later because Build 1 requires fixing the existing code without changing the architecture.
