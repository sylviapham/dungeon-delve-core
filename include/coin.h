#ifndef COIN_H
#define COIN_H

#include <stdexcept>

class Coin {
private:
    int amount_;

public:
    explicit Coin(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("Coin cannot be negative");
        }
        amount_ = amount;
    }

    int total() const {
        return amount_;
    }

    Coin operator+(const Coin& other) const {
        return Coin(amount_ + other.amount_);
    }
};

#endif
