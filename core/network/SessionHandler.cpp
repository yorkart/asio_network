#include "SessionHandler.h"
#include "AsyncAcceptor.h"
#include <cstdlib>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "../../protocol/protocol_network.h"

#define LOG4Z_FORMAT_INPUT_ENABLE

#include "../../libs/log4z.h"

SessionHandler::SessionHandler(boost::asio::io_service &io_service, AsyncAcceptor *acceptor)
        : m_socket(io_service), m_acceptor(acceptor), m_header_size(0) {
    //calculate packet header bytes size
    MSG_COMMON_REQ_FIELDS(MSG_COMMON, obj);
    fields_size(obj, 0, &m_header_size, 0, 0);
}

SessionHandler::~SessionHandler() = default;

void SessionHandler::start() {
    boost::asio::ip::tcp::endpoint remote_addr = m_socket.remote_endpoint();
    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Remote client connect!,IP:%s:%d", remote_addr.address().to_string().c_str(),
                remote_addr.port());

    m_recv_buffer.clear();
    m_recv_buffer.resize(m_header_size);
    m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0], m_header_size),
                             boost::bind(&SessionHandler::handle_read,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred,
                                         m_header_size)
    );
}

void SessionHandler::close() {
    boost::asio::ip::tcp::endpoint remote_addr = m_socket.remote_endpoint();
    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Disconnect!,Client IP:%s:%d",
                remote_addr.address().to_string().c_str(), remote_addr.port());

    m_acceptor->remove_session_handler(this);

    delete this;
}

void SessionHandler::handle_read(const boost::system::error_code &error, size_t bytes_transferred, size_t want_bytes) {
    if (!error) {
        if (bytes_transferred <= 0) {
            close();
        } else if (bytes_transferred < want_bytes) {
            //recv data for next partment
            //size_t base=m_recv_buffer.
            m_socket.async_read_some(
                    boost::asio::buffer(&m_recv_buffer[bytes_transferred], want_bytes - bytes_transferred),
                    boost::bind(&SessionHandler::handle_read,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred,
                                want_bytes - bytes_transferred)
            );
        } else {
            char *p = &m_recv_buffer[0];
            int32_t mark = *(reinterpret_cast<int32_t *>(p));
            if (HEAD_MARK == mark) {
                handle_packet_header();
            } else if (BODY_MARK == mark) {
                handle_packet_body();
            } else {
                LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID, "Unknow packet mark 0x%08x", mark);
            }
        }
    } else {
        close();
    }
}

void SessionHandler::handle_write(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error) {
        m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0], m_header_size),
                                 boost::bind(&SessionHandler::handle_read,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred,
                                             m_header_size));
    } else {
        close();
    }
}

int SessionHandler::handle_packet_header() {
    MSG_COMMON_REQ_FIELDS(MSG_COMMON, robj);
    copy_fields(m_packet_header, robj, true);

    int ret = read_packet_header();
    if (0 == ret) {
        int body_size = m_packet_header[FIELD_BSIZE].ivalue;

        m_recv_buffer.clear();
        m_recv_buffer.resize(body_size);
        m_socket.async_read_some(boost::asio::buffer(&m_recv_buffer[0], body_size),
                                 boost::bind(&SessionHandler::handle_read,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred,
                                             body_size)
        );

    } else {
        LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID, "Error in function read_packet_header(),return code: 0x%08x", ret);
    }

    return ret;
}

int SessionHandler::read_packet_header() {
    IStreambuf b((char *) (&m_recv_buffer[0]), m_header_size);
    InputStream rd(&b);
    return unserialize_header_packet(rd, m_packet_header, true);
}

int SessionHandler::handle_packet_body() {
    int method = m_packet_header[FIELD_METHOD].ivalue;
    switch (method) {
        case MSG_CONNECT:
            return msg_connect();
        case MSG_DISCONNECT:
            return msg_disconnect();
        default:
            return -1;
    }
}

int SessionHandler::read_packet_body(FIELD *fields, int len) {
    copy_fields(fields, m_packet_header, true);
    int body_size = m_packet_header[FIELD_BSIZE].ivalue;

    IStreambuf b(&m_recv_buffer[0], body_size);
    InputStream rd(&b);
    return unserialize_body_packet(rd, fields, len, true);
}

int SessionHandler::send_packet(FIELD *fields, int len) {
    int total = 0, header = 0, body = 0, body_count = 0;
    fields_size(fields, &total, &header, &body, &body_count);

    fields[FIELD_BSIZE].ivalue = body;

    m_send_buffer.clear();
    m_send_buffer.resize(total);

    OStreambuf b(&m_send_buffer[0], total);
    OutputStream wd(&b);
    int ret = serialize_packet(wd, fields, len);
    if (0 == ret) {
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(&m_send_buffer[0], total),
                                 boost::bind(&SessionHandler::handle_write, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    } else {
        LOGFMT_ERROR(LOG4Z_MAIN_LOGGER_ID, "Error in function encode_packet(),return code: 0x%08x", ret);
    }
    return ret;
}

int SessionHandler::msg_connect() {
    int ret = 0;
    char clientname[1024] = {0};
    char clientprocess[1024] = {0};

    {
        MSG_CONNECT_FIELDS(clientname, clientprocess, robj);
        ret = read_packet_body(robj, FIELDS_LEN(robj));
    }

    if (ret != ERR_OK)
        return ret;

    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Client hostname:[%s],process[%s] is connected!", clientname, clientprocess);

    MSG_CONNECT_RESP_FIELDS(ERR_OK, true, wobj);
    return send_packet(wobj, FIELDS_LEN(wobj));
}

int SessionHandler::msg_disconnect() {
    int ret = 0;

    {
        MSG_DISCONNECT_FIELDS(robj);
        ret = read_packet_body(robj, FIELDS_LEN(robj));
    }

    if (ret != ERR_OK)
        return ret;

    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Client Request disconnect now!");
    close();

    return 0;
}
