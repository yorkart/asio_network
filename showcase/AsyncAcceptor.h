//
// Created by Yorkart Wang on 2020/9/1.
//

#ifndef ASIO_NETWORK_ASYNCACCEPTOR_H
#define ASIO_NETWORK_ASYNCACCEPTOR_H

#include <boost/asio/io_service.hpp>
#include "SessionHandler.h"

namespace Showcase {
    class AsyncAcceptor {
    public:
        AsyncAcceptor(boost::asio::io_service &io_service, short port);

        ~AsyncAcceptor();

        void start_accept();

        void handle_accept(SessionHandler *handler, const boost::system::error_code &error);

    private:
        boost::asio::io_service &io_service_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };
}

#endif //ASIO_NETWORK_ASYNCACCEPTOR_H
