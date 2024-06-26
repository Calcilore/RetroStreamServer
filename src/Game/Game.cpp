#include "Game.h"

#include <utility>

#include "Snake.h"
#include "TestGame.h"
#include "../Server.h"

constexpr uint8_t FIRST_HALF = 0b11110000;
constexpr uint8_t SECOND_HALF = 0b00001111;

Game::Game(Server* server) {
    this->server = server;

    for (unsigned char& pixel : pixelData) {
        pixel = 0;
    }

    code = static_cast<int>(
        static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) % 1000000);
    printf("Code is: %d\n", code);
}

void Game::DrawPixel(const Vec2I pos, const uint8_t color) {
    DrawPixel(pos.y * GAME_WIDTH + pos.x, color);
}

void Game::DrawPixel(const int index, const uint8_t color) {
    const int realIndex = (index / 2) % PIXEL_DATA_SIZE;
    const uint8_t oldData = pixelData[realIndex];

    if (index % 2 == 0) { // Which half of the byte to write to
        // keep second half of oldData while replacing first half of new data
        pixelData[realIndex] = (oldData & SECOND_HALF) | (color << 4 & FIRST_HALF);
    } else {
        // keep first half of oldData while replacing second half of new data
        pixelData[realIndex] = (oldData & FIRST_HALF) | (color & SECOND_HALF);
    }
}

void Game::DrawRect(const Vec2I pos, const Vec2I size, const uint8_t color) {
    const uint8_t doubleColor = color | (color << 4);
    const Vec2I end = pos+size;

    const bool oddStart = pos.x % 2;
    const bool oddEnd = end.x % 2;

    const int startIndex = (pos.x + oddStart) / 2;
    const int endIndex = (end.x - oddEnd) / 2;

    for (int y = pos.y; y < end.y; ++y) {
        for (int x = startIndex; x < endIndex; ++x) {
            pixelData[x + y*GAME_WIDTH/2] = doubleColor;
        }

        if (oddStart) {
            DrawPixel(Vec2I(pos.x, y), color);
        }

        if (oddEnd) {
            DrawPixel(Vec2I(end.x-1, y), color);
        }
    }
}

void Game::ClearScreen(uint8_t color) {
    color = color | (color << 4);

    for (unsigned char& pixel : pixelData) {
        pixel = color;
    }
}

void Game::AddPlayer(udp::endpoint ep) {
    printf("Adding player to game!\n");
    addPlayer(std::move(ep));
    players.push_back(ep);
}

void Game::RemovePlayer(udp::endpoint ep) {
    printf("Removing player from game!\n");
    removePlayer(std::move(ep));
    players.erase(std::find(players.begin(), players.end(), ep));

    if (players.empty()) {
        removeGame();
        server->RemoveGame(this);
    }
}

void Game::PlayerInput(udp::endpoint ep, const bool pressed, const uint8_t input) {
    playerInput(std::move(ep), pressed, input);
}

void Game::Update() {
    update();
}

Game* Game::CreateFromId(Server* server, const std::string& gameId) {
    if (gameId == "test_game") return new TestGame(server);
    if (gameId == "snake") return new Snake(server);

    return nullptr;
}
