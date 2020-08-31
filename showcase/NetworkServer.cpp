#include "NetworkServer.h"

namespace Showcase {
    NetworkServer::NetworkServer(boost::asio::io_service &io_service, short port)
            : io_service_(io_service),
              acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

    void NetworkServer::start_accept() {
        SessionHandler *new_session = new SessionHandler(io_service_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&NetworkServer::handle_accept,
                                           this,
                                           new_session,
                                           boost::asio::placeholders::error));
    }

    void
    NetworkServer::handle_accept(SessionHandler *new_session,
                                 const boost::system::error_code &error) {
        if (!error) {
            new_session->start();
        } else {
            delete new_session;
        }

        start_accept();
    }
}