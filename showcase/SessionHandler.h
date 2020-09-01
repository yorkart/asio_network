#ifndef ASIO_NETWORK_SESSIONHANDLER_H
#define ASIO_NETWORK_SESSIONHANDLER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "PacketProtocol.h"

using boost::asio::ip::tcp;

namespace Showcase {
    class SessionHandler {
    public:
        SessionHandler(boost::asio::io_service &io_service);

        tcp::socket &socket();

        void start();

        void close();

        virtual void on_message(const char* buffer, size_t length);

    private:
        void handle_read(const boost::system::error_code &error,
                         size_t bytes_transferred,
                         size_t want_bytes,
                         size_t mark);

        void handle_write(const boost::system::error_code &error);

        tcp::socket socket_;
        enum {
            max_length = 1024
        };
        char data_[max_length];

        PacketProtocol packet_protocol_;
    };
}

#endif //ASIO_NETWORK_SESSIONHANDLER_H
