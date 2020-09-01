#include "SessionHandler.h"

namespace Showcase {
    SessionHandler::SessionHandler(boost::asio::io_service &io_service)
            : socket_(io_service), packet_protocol_(4, 1024 * 1024) {
    }

    tcp::socket &SessionHandler::socket() {
        return socket_;
    }

    void SessionHandler::start() {
        socket_.async_read_some(
                boost::asio::buffer(this->packet_protocol_.get_buffer(0),
                                    this->packet_protocol_.get_header_length()),
                boost::bind(&SessionHandler::handle_read,
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred,
                            this->packet_protocol_.get_header_length(),
                            1));
    }

    void SessionHandler::handle_read(const boost::system::error_code &error,
                                     size_t bytes_transferred,
                                     size_t want_bytes,
                                     size_t mark) {
        if (!error) {
            if (bytes_transferred <= 0) {
                this->close();
            } else if (bytes_transferred < want_bytes) {
                socket_.async_read_some(
                        boost::asio::buffer(this->packet_protocol_.get_buffer(bytes_transferred),
                                            want_bytes - bytes_transferred),
                        boost::bind(&SessionHandler::handle_read,
                                    this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred,
                                    want_bytes - bytes_transferred,
                                    mark));
            } else {
                if (mark == 1) {
                    int header_length = this->packet_protocol_.get_header_length();
                    int body_length = this->packet_protocol_.get_body_length();
                    this->packet_protocol_.reset_buffer(body_length);

                    socket_.async_read_some(
                            boost::asio::buffer(this->packet_protocol_.get_buffer(header_length),
                                                body_length),
                            boost::bind(&SessionHandler::handle_read,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred,
                                        body_length,
                                        2));
                } else {
                    printf("abc");
                }
            }

//            boost::asio::async_write(socket_,
//                                     boost::asio::buffer(data_, bytes_transferred),
//                                     boost::bind(&SessionHandler::handle_write,
//                                                 this,
//                                                 boost::asio::placeholders::error));
        } else {
            this->close();
        }
    }

    void SessionHandler::handle_write(const boost::system::error_code &error) {
        if (!error) {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&SessionHandler::handle_read,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred, 0, 1));
        } else {
            delete this;
        }
    }

    void SessionHandler::close() {
        boost::asio::ip::tcp::endpoint remote_addr = socket_.remote_endpoint();
        delete this;
    }
}