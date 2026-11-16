# Build Sprint 04 — Polymorphism

## 1. Old vs. New Ability Resolver

The original Dungeon Delve ability system used a `switch` statement to determine which action a monster performed. The `resolve_ability()` function checked an integer ability tag and executed the corresponding code.

**Original resolver: 100 lines (including blank lines).**

Example from the original resolver:

```cpp
switch (tag) {
    case ABIL_NONE:
        amount = resolve_attack_on_hero(self, hero);
        break;

    case ABIL_MEND:
        amount = roll_dice(2, 4);
        self.hp += amount;
        if (self.hp > self.max_hp)
            self.hp = self.max_hp;
        break;
}
```

The new system uses an abstract `Ability` class with virtual functions. Each concrete ability overrides `apply()` to provide its own behavior.

**New polymorphic dispatch: 1 line.**

```cpp
total += m->ability->apply(*m, hero, wave, verbose);
```

Instead of checking the ability tag during combat, the program calls `apply()` through the monster's ability pointer. C++ automatically selects the correct implementation at runtime.

The dispatch itself is shorter, although the full implementation still requires separate ability classes and a factory.

## 2. Class Hierarchy

The abstract base class `Ability` declares two pure virtual functions:

- `describe()` returns the ability's description.
- `apply()` performs the ability's action.

The concrete abilities are:

- `FlailAbility` — normal attack.
- `CleaveAbility` — attack with additional damage.
- `BiteAbility` — attack with a chance of bonus damage.
- `MendAbility` — restores health.
- `HexAbility` — reduces the hero's strength.
- `SummonAbility` — adds another monster to the wave.
- `GuardAbility` — increases armor.
- `RallyAbility` — increases the strength of other living monsters.

The hierarchy also demonstrates overriding at two levels:

`Ability → FlailAbility → CleaveAbility`

Both `FlailAbility` and `CleaveAbility` override `apply()`.

## 3. Exception Handling

The new system uses exceptions to represent ability failures.

For example, `MendAbility` throws an exception when the monster is already at full health. `HexAbility` throws an exception when the hero's strength cannot be reduced further, and `SummonAbility` throws an exception when no summons remain.

The combat loop catches exceptions using `const std::exception&`, preventing an ability failure from terminating the entire combat round.

## 4. What Became More Complicated?

Although polymorphism made the combat resolver shorter, it also made the overall design more complicated. The original system kept most ability behavior in one function, making it easier to see everything in one place. The new system requires a base class, multiple derived classes, a separate implementation file, and ability objects assigned to monsters. Debugging can also be more difficult because the behavior is spread across different classes. For example, I encountered a segmentation fault when a monster did not have an ability object assigned. I fixed this by adding a default `FlailAbility`. Despite the additional setup, the polymorphic design makes individual abilities easier to maintain and extend.

## 5. Open/Closed Principle

The open/closed principle means that software should be open for extension but closed for modification.

With the original switch-based design, adding a sixth special ability would require modifying the existing `resolve_ability()` function to add another case.

With the polymorphic design, a new ability can be created by deriving another class from `Ability` and implementing its virtual functions. The combat resolution loop does not need to change.

**Existing files that would need modification when adding a sixth special ability:**

| Design | Existing files changed |
|---|---|
| Original switch-based resolver | At least 1: `src/dungeon_delve.cpp` |
| Polymorphic resolver | 1: `src/dungeon_delve.cpp` (the ability factory) |

The polymorphic version still requires updating the factory to register the new ability. However, it does not require changing the combat resolution loop itself. This separates the creation of abilities from their execution.

## 6. Testing

After implementing the ability system, I built the project using CMake and ran the existing regression tests using CTest.

All three regression tests passed:

- `encounter_resolves`
- `loot_table_intact`
- `roster_survives_reallocation`

The tests helped identify and fix a null ability pointer that caused a segmentation fault during combat.

## Conclusion

The polymorphic ability system replaces a large tag-based combat resolver with virtual function calls. While the new approach requires more classes and additional setup, it separates ability behaviors, supports different implementations through a shared interface, and allows new abilities to be added without modifying the combat resolution loop.
