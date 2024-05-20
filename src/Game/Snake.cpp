#include "Snake.h"

void Snake::update() {
    ++frameCount;
    if (frameCount % 2 != 0) {
        return;
    }

    ClearScreen(0);

    for (auto& [id, player] : players) {
        const Vec2I pos = player.segments.front();

        if (!player.inputQueue.empty()) {
            player.dir = player.inputQueue.front();
            player.inputQueue.pop_front();
        }

        const Vec2I newPos = Vec2I(
            positiveModulo(pos.x + player.dir.x, GAME_WIDTH),
            positiveModulo(pos.y + player.dir.y, GAME_HEIGHT)
        );

        if (!checkCollision(&id, newPos)) {
            player.segments.push_front(newPos);
        }

        if (newPos == apple) {
            player.growAmount += 6;

            int i = 0;
            do {
                apple = Vec2I(generateNumber(GAME_WIDTH), generateNumber(GAME_HEIGHT));
                ++i;
            } while (i < 100 && checkCollision(nullptr, apple));
        }

        if (player.growAmount > 0) {
            --player.growAmount;
        } else {
            player.segments.pop_back();
        }

        for (const Vec2I pixel : player.segments) {
            SetPixel(pixel, player.color);
        }

        if (player.segments.empty()) {
            player = Player(player.color);
        }
    }

    SetPixel(apple, 0xf);
}

void Snake::playerInput(const udp::endpoint ep, const bool pressed, const uint8_t input) {
    if (!pressed) {
        return;
    }

    Player* p = &players[ep];
    const Vec2I dir = p->inputQueue.empty() ? p->dir : p->inputQueue.front();

    switch (input) {
        case 0: // up
            if (dir == Vec2I(0,  1)) return;
            p->inputQueue.emplace_back(0, -1);
            break;

        case 1: // down
            if (dir == Vec2I(0, -1)) return;
            p->inputQueue.emplace_back(0,  1);
            break;

        case 2: // left
            if (dir == Vec2I( 1, 0)) return;
            p->inputQueue.emplace_back(-1, 0);
            break;

        case 3: // right
            if (dir == Vec2I(-1, 0)) return;
            p->inputQueue.emplace_back( 1, 0);
            break;

        default: break;
    }
}

void Snake::addPlayer(const udp::endpoint ep) {
    players[ep] = Player(players.size() % 15 + 2);
}

void Snake::removePlayer(const udp::endpoint ep) {
    players.erase(ep);
}

int Snake::positiveModulo(const int number, const int modulus) {
    return (number % modulus + modulus) % modulus;
}

int Snake::generateNumber(const int bound) {
    return static_cast<int>(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) % bound);
}

bool Snake::checkCollision(const udp::endpoint* id, const Vec2I newPos) {
    for (auto& [collId, collidingPlayer] : players) {
        for (auto segment : collidingPlayer.segments) {
            if (newPos == segment) {
                if (id != nullptr && collId != *id) {
                    ++collidingPlayer.growAmount;
                }
                return true;
            }
        }
    }

    return false;
}
