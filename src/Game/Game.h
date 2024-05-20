#ifndef GAME_H
#define GAME_H

#include <boost/asio.hpp>
#include "../Engine/Vec2I.h"

class Server;
using boost::asio::ip::udp;

constexpr int GAME_WIDTH = 64;
constexpr int GAME_HEIGHT = 48;
constexpr int GAME_AREA = GAME_WIDTH*GAME_HEIGHT;
constexpr int PIXEL_DATA_SIZE = GAME_AREA/2; // 2 pixels per byte (just 16 colors per pixel)

class Game {
private:
    Server* server;

    virtual void update() {}
    virtual void addPlayer(udp::endpoint ep) {}
    virtual void removePlayer(udp::endpoint ep) {}
    virtual void removeGame() {}
    virtual void playerInput(udp::endpoint ep, bool pressed, uint8_t input) {}

protected:
    void SetPixel(Vec2I pos, uint8_t color);
    void SetPixel(int pos, uint8_t color);
    void ClearScreen(uint8_t color);

public:
    uint8_t pixelData[PIXEL_DATA_SIZE] = {};
    std::vector<udp::endpoint> players = {};
    int code = 0;

    explicit Game(Server* server);
    virtual ~Game() = default;
    static Game* CreateFromId(Server* server, const std::string& gameId);

    void Update();
    void AddPlayer(udp::endpoint ep);
    void RemovePlayer(udp::endpoint ep);
    void PlayerInput(udp::endpoint ep, bool pressed, uint8_t input);

};

#endif //GAME_H
