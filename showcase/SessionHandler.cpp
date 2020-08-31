#include "SessionHandler.h"

namespace Showcase {
    SessionHandler::SessionHandler(boost::asio::io_service &io_service)
            : socket_(io_service) {
    }

    tcp::socket &SessionHandler::socket() {
        return socket_;
    }

    void SessionHandler::start() {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&SessionHandler::handle_read,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void SessionHandler::handle_read(const boost::system::error_code &error, size_t bytes_transferred) {
        if (!error) {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(data_, bytes_transferred),
                                     boost::bind(&SessionHandler::handle_write,
                                                 this,
                                                 boost::asio::placeholders::error));
        } else {
            delete this;
        }
    }

    void SessionHandler::handle_write(const boost::system::error_code &error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&SessionHandler::handle_read,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }
}