
#pragma once

#include "dungeon_delve.h"
#include <string>
#include <stdexcept>

// Abstract base class for all abilities.
class Ability {
public:
    virtual ~Ability() = default;

    virtual std::string describe() const = 0;
    virtual int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const = 0;
};

// Dispatches through the abstract Ability interface.
inline int dispatch_ability(
    const Ability& ability,
    Monster& self,
    Hero& hero,
    Wave& wave,
    int verbose)
{
    return ability.apply(self, hero, wave, verbose);
}

// Default attack for monsters without a special ability.
class FlailAbility : public Ability {
public:
    std::string describe() const override { return "Flail"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

// Second-level override: Ability -> FlailAbility -> CleaveAbility.
class CleaveAbility : public FlailAbility {
public:
    std::string describe() const override { return "Cleave"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class BiteAbility : public Ability {
public:
    std::string describe() const override { return "Bite"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class MendAbility : public Ability {
public:
    std::string describe() const override { return "Mend"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class HexAbility : public Ability {
public:
    std::string describe() const override { return "Hex"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class SummonAbility : public Ability {
public:
    std::string describe() const override { return "Summon"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class GuardAbility : public Ability {
public:
    std::string describe() const override { return "Guard"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};

class RallyAbility : public Ability {
public:
    std::string describe() const override { return "Rally"; }
    int apply(Monster& self, Hero& hero, Wave& wave, int verbose) const override;
};
