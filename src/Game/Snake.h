#ifndef SNAKE_H
#define SNAKE_H
#include <list>
#include <map>

#include "Game.h"

class Snake : public Game {
private:
    class Player {
    public:
        std::list<Vec2I> segments{};
        std::list<Vec2I> inputQueue{};
        uint8_t color = 1;
        Vec2I dir;
        int growAmount = 0;

        Player() = default;
        explicit Player(const uint8_t color) {
            this->color = color;
            this->dir = Vec2I(1, 0);

            for (int i = 0; i < 6; ++i) {
                this->segments.emplace_front(i, 0);
            }
        }
    };

protected:
    void update() override;
    void playerInput(udp::endpoint ep, bool pressed, uint8_t input) override;
    void addPlayer(udp::endpoint ep) override;
    void removePlayer(udp::endpoint ep) override;

public:
    explicit Snake(Server* server) : Game(server) {}

private:
    std::map<udp::endpoint, Player> players;
    Vec2I apple = Vec2I(5, 5);
    int frameCount = 0;

    static int positiveModulo(int number, int modulus);
    static int generateNumber(int bound);
    bool checkCollision(const udp::endpoint* id, Vec2I newPos);
};



#endif //SNAKE_H
