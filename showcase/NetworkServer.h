
#ifndef ASIO_NETWORK_NETWORKSERVER_H
#define ASIO_NETWORK_NETWORKSERVER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "SessionHandler.h"

using boost::asio::ip::tcp;

namespace Showcase {
    class NetworkServer {
    public:
        NetworkServer(boost::asio::io_service &io_service, short port);

    private:
        void start_accept();

        void handle_accept(Showcase::SessionHandler *new_session,
                           const boost::system::error_code &error);

        boost::asio::io_service &io_service_;
        tcp::acceptor acceptor_;
    };
}

#endif //ASIO_NETWORK_NETWORKSERVER_H
