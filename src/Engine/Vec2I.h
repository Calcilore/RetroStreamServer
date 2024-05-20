#ifndef VEC2I_H
#define VEC2I_H

class Vec2I {
    public:
        int x;
        int y;

        Vec2I(int x, int y);
        Vec2I();

        Vec2I operator+(const Vec2I& other) const;
        Vec2I operator-(const Vec2I& other) const;
        Vec2I operator*(const Vec2I& other) const;
        Vec2I operator/(const Vec2I& other) const;

        Vec2I operator+(const int& other) const;
        Vec2I operator-(const int& other) const;
        Vec2I operator*(const int& other) const;
        Vec2I operator/(const int& other) const;

        Vec2I operator+=(const Vec2I& other);
        Vec2I operator-=(const Vec2I& other);
        Vec2I operator*=(const Vec2I& other);
        Vec2I operator/=(const Vec2I& other);

        Vec2I operator+=(const int& other);
        Vec2I operator-=(const int& other);
        Vec2I operator*=(const int& other);
        Vec2I operator/=(const int& other);

        bool operator==(const Vec2I& other) const;
        bool operator!=(const Vec2I& other) const;

        [[nodiscard]] Vec2I Normalized() const;
        void Normalize();
        [[nodiscard]] float Magnitude() const;
        [[nodiscard]] float Distance(const Vec2I& other) const;
};


#endif // VEC2I_H
