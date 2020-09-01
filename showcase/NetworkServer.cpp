#include "NetworkServer.h"

namespace Showcase {
    NetworkServer::NetworkServer(short port)
            : async_acceptor_(io_service_, port) {
    }

    void NetworkServer::run() {
        try {
            async_acceptor_.start_accept();
            std::cout << "Server start OK! ^_^" << std::endl;

            io_service_.run();
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }
}