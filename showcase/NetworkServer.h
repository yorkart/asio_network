
#ifndef ASIO_NETWORK_NETWORKSERVER_H
#define ASIO_NETWORK_NETWORKSERVER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "SessionHandler.h"
#include "AsyncAcceptor.h"

using boost::asio::ip::tcp;

namespace Showcase {
    class NetworkServer {
    public:
        NetworkServer(short port);

        void run();

    private:
        boost::asio::io_service io_service_;
        AsyncAcceptor async_acceptor_;
    };
}

#endif //ASIO_NETWORK_NETWORKSERVER_H
