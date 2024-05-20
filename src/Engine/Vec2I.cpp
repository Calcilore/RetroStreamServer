#include "Vec2I.h"
#include <cmath>

Vec2I::Vec2I(int x, int y) {
    this->x = x;
    this->y = y;
}

Vec2I::Vec2I() {
    this->x = 0;
    this->y = 0;
}

Vec2I Vec2I::operator+(const Vec2I &other) const {
    return {x + other.x, y + other.y};
}

Vec2I Vec2I::operator-(const Vec2I &other) const {
    return {x - other.x, y - other.y};
}

Vec2I Vec2I::operator*(const Vec2I &other) const {
    return {x * other.x, y * other.y};
}

Vec2I Vec2I::operator/(const Vec2I &other) const {
    return {x / other.x, y / other.y};
}

Vec2I Vec2I::operator+(const int &other) const {
    return {x + other, y + other};
}

Vec2I Vec2I::operator-(const int &other) const {
    return {x - other, y - other};
}

Vec2I Vec2I::operator*(const int &other) const {
    return {x * other, y * other};
}

Vec2I Vec2I::operator/(const int &other) const {
    return {x / other, y / other};
}

Vec2I Vec2I::operator+=(const Vec2I &other) {
    x = x + other.x;
    y = y + other.y;
    return *this;
}

Vec2I Vec2I::operator-=(const Vec2I &other) {
    x = x - other.x;
    y = y - other.y;
    return *this;
}

Vec2I Vec2I::operator*=(const Vec2I &other) {
    x = x * other.x;
    y = y * other.y;
    return *this;
}

Vec2I Vec2I::operator/=(const Vec2I &other) {
    x = x / other.x;
    y = y / other.y;
    return *this;
}

Vec2I Vec2I::operator+=(const int &other) {
    x = x + other;
    y = y + other;
    return *this;
}

Vec2I Vec2I::operator-=(const int &other) {
    x = x - other;
    y = y - other;
    return *this;
}

Vec2I Vec2I::operator*=(const int &other) {
    x = x * other;
    y = y * other;
    return *this;
}

Vec2I Vec2I::operator/=(const int &other) {
    x = x / other;
    y = y / other;
    return *this;
}

Vec2I Vec2I::Normalized() const {
    int magnitude = Magnitude();
    if (magnitude == 0) {
        return {0, 0};
    }

    return {x / magnitude, y / magnitude};
}

void Vec2I::Normalize() {
    int magnitude = Magnitude();
    if (magnitude != 0) {
        x /= magnitude;
        y /= magnitude;
    }
}

float Vec2I::Magnitude() const {
    return sqrtf((float)(x * x + y * y));
}

float Vec2I::Distance(const Vec2I &other) const {
    return sqrtf(powf((float)(x - other.x), 2) + powf((float)(y - other.y), 2));
}

bool Vec2I::operator==(const Vec2I &other) const {
    return x == other.x && y == other.y;
}

bool Vec2I::operator!=(const Vec2I &other) const {
    return x != other.x || y != other.y;
}
