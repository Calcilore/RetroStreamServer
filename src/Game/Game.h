#ifndef GAME_H
#define GAME_H

#include <boost/asio.hpp>

#include "../Engine/Color.h"
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
    void DrawPixel(Vec2I pos, uint8_t color);
    void DrawPixel(int pos, uint8_t color);
    void DrawRect(Vec2I pos, Vec2I size, uint8_t color);
    void ClearScreen(uint8_t color);

public:
    Color palette[16] = {
        Color{255, 255, 255}, Color{0, 0, 0}, Color{230, 41, 55}, Color{0, 228, 48},
        Color{0, 121, 241}, Color{253, 249, 0}, Color{255, 161, 0}, Color{200, 122, 255},
        Color{130, 130, 130}, Color{80, 80, 80}, Color{190, 33, 55}, Color{0, 117, 44},
        Color{0, 82, 172}, Color{255, 203, 0}, Color{127, 106, 79}, Color{255, 109, 194}};

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
