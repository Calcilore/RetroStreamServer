#ifndef TESTGAME_H
#define TESTGAME_H
#include "Game.h"


class TestGame : public Game {
    uint8_t color = 0;
    int index = 0;

    void update() override;
    void addPlayer(udp::endpoint ep) override;
    void playerInput(udp::endpoint ep, bool pressed, uint8_t input) override;

public:
    explicit TestGame(Server* server) : Game(server) {}
};

#endif //TESTGAME_H
