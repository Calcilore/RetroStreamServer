#include "Server.h"

#include <boost/asio.hpp>
#include <chrono>
#include <thread>

#include "PacketTypes.h"
#include "Game/Snake.h"

using namespace std::chrono;

Server::Server(const int port):
socket(io_context, udp::endpoint(udp::v4(), port)) {
    startReceive();
}

void Server::startReceive() {
    socket.async_receive_from(
    boost::asio::buffer(recv_buffer), remote_endpoint,
    std::bind(&Server::handleReceive, this,
              std::placeholders::_1,
              std::placeholders::_2));
}

void Server::handleReceive(const boost::system::error_code& error, const std::size_t size) {
    if (error) {
        return;
    }

    if (size == 0) {
        printf("Empty Packet Recieved?\n");
        startReceive();
        return;
    }

    switch (recv_buffer[0]) {
        case C2S_CREATE_GAME: {
            const std::string gameId(reinterpret_cast<char*>(&recv_buffer[1]), size-1);

            if (Game* existingGame = getGameFromPlayer(); existingGame != nullptr) {
                existingGame->RemovePlayer(remote_endpoint);
                printf("Player was already in a game, removing.\n");
            }

            Game* game = Game::CreateFromId(this, gameId);
            if (game == nullptr) {
                printf("Invalid game id!\n");

                constexpr std::array<unsigned char, 1> packet{S2C_CONNECT_FAIL};
                sendPacket(boost::asio::const_buffer(packet.begin(), packet.size()), remote_endpoint);
                break;
            }

            game->AddPlayer(remote_endpoint);
            games.push_back(game);
            playerGames[remote_endpoint] = game;

            // send packet saying it connected
            std::array<unsigned char, 5> createdPacket{S2C_GAME_START};
            const uint32_t netorder = htonl(game->code); // include code so client knows the code
            std::memcpy(&createdPacket[1], &netorder, sizeof(netorder));
            sendPacket(boost::asio::const_buffer(createdPacket.begin(), createdPacket.size()), remote_endpoint);

            printf("Created new game!\n");
            break;
        }

        case C2S_EXIT_GAME: { // Exit game
            if (Game* existingGame = getGameFromPlayer(); existingGame != nullptr) {
                existingGame->RemovePlayer(remote_endpoint);
                playerGames.erase(remote_endpoint);
            }
            break;
        }

        case C2S_SEND_INPUTS: { // Send inputs
            if (Game* existingGame = getGameFromPlayer(); existingGame != nullptr) {
                existingGame->PlayerInput(remote_endpoint, recv_buffer[1], recv_buffer[2]);
            } else {
                printf("Player sent input when not in a game?\n");
            }
            break;
        }

        case C2S_JOIN_GAME: { // Join existing game
            uint32_t network_order;
            std::memcpy(&network_order, &recv_buffer[1], sizeof(network_order));
            const int code = static_cast<int>(ntohl(network_order));

            Game* game = nullptr;
            for (Game* existingGame : games) {
                if (existingGame->code == code) {
                    game = existingGame;
                    break;
                }
            }

            if (game == nullptr) {
                printf("Invalid game code!\n");

                constexpr std::array<unsigned char, 1> packet{S2C_CONNECT_FAIL};
                sendPacket(boost::asio::const_buffer(packet.begin(), packet.size()), remote_endpoint);
                break;
            }

            game->AddPlayer(remote_endpoint);
            playerGames[remote_endpoint] = game;
            printf("Joined existing game!\n");
            break;
        }

        default: { // o no
            printf("Incorrect packet type!\n");
            break;
        }
    }

    startReceive();
}

void Server::handleSend(const boost::system::error_code& error, std::size_t) {
    if (!error.failed()) {
        return;
    }

    if (Game* existingGame = getGameFromPlayer(); existingGame != nullptr) {
        existingGame->RemovePlayer(remote_endpoint);
        playerGames.erase(remote_endpoint);
    }
}

void Server::sendPacket(const boost::asio::const_buffer buffer, const udp::endpoint& endpoint) {
    socket.async_send_to(buffer, endpoint,
      std::bind(&Server::handleSend, this,
                std::placeholders::_1,
                std::placeholders::_2));
}

Game* Server::getGameFromPlayer() const {
    try {
        Game* existingGame = playerGames.at(remote_endpoint);
        return existingGame;
    } catch (std::out_of_range&) { /* didnt find it */ }

    return nullptr;
}

void Server::Loop() {
    printf("Started Server!\n");

    while (keepRunning) {
        auto start = high_resolution_clock::now();

        // UPDATE:
        io_context.poll();

        for (Game* game : games) {
            game->Update();

            for (const auto& player : game->players) {
                // DRAW:
                // Len: Packet Type + Palette + Pixel Data
                std::array<uint8_t, 1+16*3+PIXEL_DATA_SIZE> rawBuffer{};
                rawBuffer[0] = S2C_UPDATE_DATA;
                std::memcpy(rawBuffer.begin()+1, game->palette, 16*3);
                std::memcpy(rawBuffer.begin()+1+16*3, game->pixelData, PIXEL_DATA_SIZE);

                const boost::asio::const_buffer buffer(rawBuffer.begin(), rawBuffer.size());
                sendPacket(buffer, player);
            }
        }

        // END UPDATE:
        time_point<system_clock> end = high_resolution_clock::now();
        duration<signed long int, std::ratio<1, 1000000>> duration = duration_cast<microseconds>(end - start);
        const long delay = (33333) - duration.count(); // 33333 microseconds = 1/30 seconds

        if (delay > 0) {
            std::this_thread::sleep_for(microseconds(delay));
        }
    }
}

void Server::RemoveGame(const Game* game) {
    printf("Removing game...\n");
    games.erase(std::find(games.begin(), games.end(), game));
    delete game;
}
