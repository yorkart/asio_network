#ifndef __NETWORK_SERVER_HEADER_H_
#define __NETWORK_SERVER_HEADER_H_

#include "async_acceptor.h"

class network_server {
public:
    network_server(int port, int conn_num);

    ~network_server();

    void run();

    void stop();

protected:
    network_server(const network_server &);

private:
    boost::asio::io_service m_net_server;
    async_acceptor m_net_acceptor;
};

#endif
