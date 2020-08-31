// https://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/example/echo/async_tcp_echo_server.cpp

#include <iostream>
#include <boost/asio/io_service.hpp>

#include "NetworkServer.h"

int main0(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        Showcase::NetworkServer s(io_service, atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}