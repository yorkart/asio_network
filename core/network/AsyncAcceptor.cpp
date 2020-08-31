#include "AsyncAcceptor.h"
#include "SessionHandler.h"

#define LOG4Z_FORMAT_INPUT_ENABLE

#include "../../libs/log4z.h"

AsyncAcceptor::AsyncAcceptor(boost::asio::io_service &io_service, short port)
        : m_io_service(io_service),
          m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
}

AsyncAcceptor::~AsyncAcceptor() = default;

void AsyncAcceptor::start() {
    boost::asio::ip::tcp::endpoint ep = m_acceptor.local_endpoint();
    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Server listen on %s:%d", ep.address().to_string().c_str(), ep.port());

    SessionHandler *handler = this->make_session_handler();
    m_acceptor.async_accept(handler->socket(),
                            boost::bind(&AsyncAcceptor::handle_accept,
                                        this,
                                        handler,
                                        boost::asio::placeholders::error));

}

void AsyncAcceptor::handle_accept(SessionHandler *handler, const boost::system::error_code &error) {
    if (!error) {
        handler->start();

        handler = this->make_session_handler();
        m_acceptor.async_accept(handler->socket(),
                                boost::bind(&AsyncAcceptor::handle_accept,
                                            this,
                                            handler,
                                            boost::asio::placeholders::error));
    } else {
        remove_session_handler(handler);
        delete handler;
    }
}

SessionHandler *AsyncAcceptor::make_session_handler() {
    SessionHandler *handler = new(std::nothrow) SessionHandler(m_io_service, this);
    if (handler) {
        boost::mutex::scoped_lock locker(m_mtx_handlers);
        m_set_handlers.insert(handler);

        return handler;
    }

    return nullptr;
}

void AsyncAcceptor::remove_session_handler(SessionHandler *handler) {
    if (handler) {
        boost::mutex::scoped_lock locker(m_mtx_handlers);
        m_set_handlers.erase(handler);
    }
}
