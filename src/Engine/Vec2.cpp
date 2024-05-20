#include "Vec2.h"
#include <cmath>

Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec2::Vec2() {
    this->x = 0;
    this->y = 0;
}

Vec2 Vec2::operator+(const Vec2 &other) const {
    return {x + other.x, y + other.y};
}

Vec2 Vec2::operator-(const Vec2 &other) const {
    return {x - other.x, y - other.y};
}

Vec2 Vec2::operator*(const Vec2 &other) const {
    return {x * other.x, y * other.y};
}

Vec2 Vec2::operator/(const Vec2 &other) const {
    return {x / other.x, y / other.y};
}

Vec2 Vec2::operator+(const float &other) const {
    return {x + other, y + other};
}

Vec2 Vec2::operator-(const float &other) const {
    return {x - other, y - other};
}

Vec2 Vec2::operator*(const float &other) const {
    return {x * other, y * other};
}

Vec2 Vec2::operator/(const float &other) const {
    return {x / other, y / other};
}

Vec2 Vec2::operator+=(const Vec2 &other) {
    x = x + other.x;
    y = y + other.y;
    return *this;
}

Vec2 Vec2::operator-=(const Vec2 &other) {
    x = x - other.x;
    y = y - other.y;
    return *this;
}

Vec2 Vec2::operator*=(const Vec2 &other) {
    x = x * other.x;
    y = y * other.y;
    return *this;
}

Vec2 Vec2::operator/=(const Vec2 &other) {
    x = x / other.x;
    y = y / other.y;
    return *this;
}

Vec2 Vec2::operator+=(const float &other) {
    x = x + other;
    y = y + other;
    return *this;
}

Vec2 Vec2::operator-=(const float &other) {
    x = x - other;
    y = y - other;
    return *this;
}

Vec2 Vec2::operator*=(const float &other) {
    x = x * other;
    y = y * other;
    return *this;
}

Vec2 Vec2::operator/=(const float &other) {
    x = x / other;
    y = y / other;
    return *this;
}

Vec2 Vec2::Normalized() const {
    float magnitude = Magnitude();
    if (magnitude == 0) {
        return {0, 0};
    }

    return {x / magnitude, y / magnitude};
}

void Vec2::Normalize() {
    float magnitude = Magnitude();
    if (magnitude != 0) {
        x /= magnitude;
        y /= magnitude;
    }
}

float Vec2::Magnitude() const {
    return sqrtf(x * x + y * y);
}

float Vec2::Distance(const Vec2 &other) const {
    return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2));
}
