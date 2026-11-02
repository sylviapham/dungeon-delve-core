#ifndef VEC2_H
#define VEC2_H

#include <stdexcept>

class Vec2 {
private:
    int x_;
    int y_;

public:
    Vec2(int x, int y) {
        if (x < 0 || y < 0) {
            throw std::invalid_argument(
                "Position coordinates cannot be negative"
            );
        }

        x_ = x;
        y_ = y;
    }

    int x() const {
        return x_;
    }

    int y() const {
        return y_;
    }

    bool operator==(const Vec2& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
};

#endif
