# Build 3 — Entity Hierarchy

## Hierarchy Diagram

```text
Describable
├── Entity (virtual)
│   ├── Hero
│   ├── Monster
│   └── Merchant
├── Combatant (virtual)
│   ├── Hero
│   └── Monster
└── Lootable (virtual)
    ├── Monster
    └── Merchant
```

## Design Decision

The most debatable decision was making Combatant and Lootable inherit from Describable instead of keeping them completely separate. I chose this because it gives all the classes a shared description interface while keeping their roles separate. Virtual inheritance prevents duplicate Describable objects when a class inherits from multiple bases. This also lets the game describe different entities without checking their types manually.

## Inheritance Edges

- Describable to Entity: `show_description(const Describable&)` can describe an Entity.
- Describable to Combatant: `show_description(const Describable&)` can describe a Combatant.
- Describable to Lootable: `show_description(const Describable&)` can describe a Lootable.
- Entity to Hero: `show_entity(const Entity&)` can describe a Hero.
- Entity to Monster: `show_entity(const Entity&)` can describe a Monster.
- Entity to Merchant: `show_entity(const Entity&)` can describe a Merchant.
- Combatant to Hero: `show_combatant(const Combatant&)` can describe a Hero.
- Combatant to Monster: `show_combatant(const Combatant&)` can describe a Monster.
- Lootable to Monster: `show_loot(const Lootable&)` can describe a Monster.
- Lootable to Merchant: `show_loot(const Lootable&)` can describe a Merchant.

These are example functions that could accept the base classes and call the virtual `describe()` method.

## Diamond Resolution

Monster inherits from Entity, Combatant, and Lootable. All three inherit virtually from Describable, which creates a diamond-shaped inheritance structure. Virtual inheritance makes sure Monster has only one Describable subobject instead of multiple copies. This prevents ambiguity when using the shared description interface.