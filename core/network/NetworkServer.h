#ifndef __NETWORK_SERVER_HEADER_H_
#define __NETWORK_SERVER_HEADER_H_

#include "AsyncAcceptor.h"

class NetworkServer {
public:
    NetworkServer(int port, int conn_num);

    ~NetworkServer();

    void run();

    void stop();

protected:
    NetworkServer(const NetworkServer &);

private:
    boost::asio::io_service m_net_server;
    AsyncAcceptor m_net_acceptor;
};

#endif
