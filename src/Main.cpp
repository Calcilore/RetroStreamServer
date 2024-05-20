#include <iostream>
#include <boost/asio.hpp>
#include "Server.h"

using boost::asio::ip::udp;

void Client(const char* ip, int port);

int main(const int argc, char* argv[]) {
    if (argc == 3) {
        Client(argv[1], static_cast<int>(strtol(argv[2], nullptr, 0)));
        return 0;
    }

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    const int port = static_cast<int>(strtol(argv[1], nullptr, 0));

    try {
        Server server(port);
        server.Loop();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void Client(const char* ip, const int port) {
    try {
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint receiver_endpoint =
          *resolver.resolve(udp::v4(), ip, std::to_string(port)).begin();

        udp::socket socket(io_context);
        socket.open(udp::v4());

        std::array<unsigned char, 1> send_buf = {0};
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        std::array<unsigned char, 128> recv_buf{};
        udp::endpoint sender_endpoint;
        const size_t len = socket.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint);

        // std::cout.write(recv_buf.data(), static_cast<int>(len));
        uint8_t ch = recv_buf[0];
        std::cout << "Recieved first byte: " << static_cast<int>(ch) << "!\n";
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
