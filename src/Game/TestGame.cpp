#include "TestGame.h"

void TestGame::update() {
    for (int i = 0; i < GAME_AREA; ++i) {
        DrawPixel(i, i % 16);
    }

    // for (int i = 0; i < 1; ++i) {
    //     color = (color + 1) % 15;
    //     for (int j = 0; j < GAME_AREA; j += 5) {
    //         SetPixel(index + j, color+1);
    //         color = (color + 1) % 15;
    //     }
    //
    //     index += 1;
    // }
}

void TestGame::addPlayer(udp::endpoint ep) {

}

void TestGame::playerInput(udp::endpoint ep, const bool pressed, const uint8_t input) {
    printf("Player %d'd thier %d key\n", pressed, input);
}
