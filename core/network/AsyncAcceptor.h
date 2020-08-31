#ifndef __ASYNC_ACCEPTOR_HEADER_H__
#define __ASYNC_ACCEPTOR_HEADER_H__

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <set>

//#define ACCEPT_CONNECT_CLIENT_COUNT  100

class SessionHandler;

class AsyncAcceptor {
public:
    AsyncAcceptor(boost::asio::io_service &io_service, short port);

    virtual ~AsyncAcceptor();

    void start();

    void handle_accept(SessionHandler *handler, const boost::system::error_code &error);

    SessionHandler *make_session_handler();

    void remove_session_handler(SessionHandler *handler);

private:
    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;

    typedef std::set<SessionHandler *> SESSION_LIST;
    boost::mutex m_mtx_handlers;
    SESSION_LIST m_set_handlers;
};

#endif
