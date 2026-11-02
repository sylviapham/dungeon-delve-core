#ifndef DAMAGE_H
#define DAMAGE_H

#include <stdexcept>

class Damage {
private:
    int amount_;

public:
    explicit Damage(int amount) {
        if (amount < 0) {
            throw std::invalid_argument(
                "Damage cannot be negative"
            );
        }

        amount_ = amount;
    }

    int amount() const {
        return amount_;
    }

    bool operator==(const Damage& other) const {
        return amount_ == other.amount_;
    }
};

#endif
