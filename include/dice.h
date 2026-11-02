#ifndef DICE_H
#define DICE_H

#include <stdexcept>

class Dice {
private:
    int count_;
    int sides_;

public:
    Dice(int count, int sides) {
        if (count <= 0 || sides <= 0) {
            throw std::invalid_argument(
                "Dice count and sides must be positive"
            );
        }

        count_ = count;
        sides_ = sides;
    }

    int count() const {
        return count_;
    }

    int sides() const {
        return sides_;
    }

    int maximum() const {
        return count_ * sides_;
    }
};

#endif
