#include "ability.h"
#include <cstdio>
#include <algorithm>
#include <stdexcept>

int CleaveAbility::apply(Monster& self, Hero& hero, Wave&, int verbose) const {
    int first = resolve_attack_on_hero(self, hero);
    int extra = first / 2;
    apply_damage_to_hero(hero, extra);
    int amount = first + extra;
    if (verbose)
        std::printf("  %s cleaves ........ %d damage\n", self.name, amount);
    return amount;
}

int BiteAbility::apply(Monster& self, Hero& hero, Wave&, int verbose) const {
    int amount = resolve_attack_on_hero(self, hero);
    if (dd_rand_range(1, 100) > 70) {
        apply_damage_to_hero(hero, 2);
        amount += 2;
    }
    if (verbose)
        std::printf("  %s bites .......... %d damage\n", self.name, amount);
    return amount;
}

int MendAbility::apply(Monster& self, Hero&, Wave&, int verbose) const {
    if (self.hp >= self.max_hp)
        throw std::runtime_error("Mend failed: already at full health");

    int amount = roll_dice(2, 4);
    int healed = std::min(amount, self.max_hp - self.hp);
    self.hp += healed;

    if (verbose)
        std::printf("  %s mends .......... +%d HP\n", self.name, healed);
    return healed;
}

int HexAbility::apply(Monster& self, Hero& hero, Wave&, int verbose) const {
    if (hero.strength <= 1)
        throw std::runtime_error("Hex failed: strength cannot go lower");

    int amount = roll_dice(1, 4);
    hero.strength = std::max(1, hero.strength - amount);

    if (verbose)
        std::printf("  %s hexes .......... -%d STR\n", self.name, amount);
    return amount;
}

int SummonAbility::apply(Monster& self, Hero& hero, Wave& wave, int verbose) const {
    if (self.summon_budget <= 0)
        throw std::runtime_error("Summon failed: no summons remaining");

    --self.summon_budget;
    wave.push_back(make_monster("Whelp", "whelp", 6, 2, 0, 3, 0));

    if (verbose)
        std::printf("  %s summons a Whelp\n", self.name);
    return 1;
}

int FlailAbility::apply(Monster& self, Hero& hero, Wave&, int verbose) const {
    int amount = resolve_attack_on_hero(self, hero);

    if (verbose)
        std::printf("  %s flails ........ %d damage\n", self.name, amount);

    return amount;
}

int GuardAbility::apply(Monster& self, Hero&, Wave&, int verbose) const {
    self.armor += 2;

    if (verbose)
        std::printf("  %s guards .......... +2 armor\n", self.name);

    return 2;
}

int RallyAbility::apply(Monster& self, Hero&, Wave& wave, int verbose) const {
    for (size_t i = 0; i < wave.size(); i++) {
        if (!wave[i]->is_dead && raw(wave[i]) != &self)
            wave[i]->strength += 1;
    }

    if (verbose)
        std::printf("  %s rallies the pack ........ +1 STR each\n", self.name);

    return 1;
}
