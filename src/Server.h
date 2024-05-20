#ifndef SERVER_H
#define SERVER_H
#include <list>
#include <map>
#include <boost/asio.hpp>

#include "Game/Game.h"

using boost::asio::ip::udp;


class Server {
public:
    bool keepRunning = true;

    explicit Server(int port);
    void Loop();
    void RemoveGame(const Game* game);

private:
    boost::asio::io_context io_context;
    udp::socket socket;
    udp::endpoint remote_endpoint;
    std::array<unsigned char, 8192> recv_buffer{};

    std::list<Game*> games;
    std::map<udp::endpoint, Game*> playerGames;

    void startReceive();
    void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handleSend(const boost::system::error_code& error, std::size_t bytes_transferred);
    void sendPacket(boost::asio::const_buffer buffer, const udp::endpoint& endpoint);
    Game* getGameFromPlayer() const;
};



#endif //SERVER_H
