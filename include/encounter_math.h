#ifndef ENCOUNTER_MATH_H
#define ENCOUNTER_MATH_H

#include "damage.h"
#include "dice.h"

class EncounterMath {
private:
    // This class has no individual state.
    // Prevent instantiation because all calculations are static.
    EncounterMath() = delete;

public:
    static Damage applyArmor(const Damage& incoming, int armor) {
        int result = incoming.amount() - armor;

        if (result < 1) {
            result = 1;
        }

        return Damage(result);
    }

    static int maximumDamage(int strength, const Dice& dice) {
        return strength + dice.maximum();
    }
};

#endif
