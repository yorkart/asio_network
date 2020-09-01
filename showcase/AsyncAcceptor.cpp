#include "AsyncAcceptor.h"

namespace Showcase {

    AsyncAcceptor::AsyncAcceptor(boost::asio::io_service &io_service, short port) :
            acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
            io_service_(io_service) {

    }

    AsyncAcceptor::~AsyncAcceptor() = default;

    void AsyncAcceptor::start_accept() {
        auto *session_handler = new SessionHandler(io_service_);
        acceptor_.async_accept(session_handler->socket(),
                               boost::bind(&AsyncAcceptor::handle_accept,
                                           this,
                                           session_handler,
                                           boost::asio::placeholders::error));
    }

    void AsyncAcceptor::handle_accept(SessionHandler *session_handler, const boost::system::error_code &error) {
        if (!error) {
            session_handler->start();
        } else {
            delete session_handler;
        }

        this->start_accept();
    }

}