#ifndef ASIO_NETWORK_PACKETPROTOCOL_H
#define ASIO_NETWORK_PACKETPROTOCOL_H

#include <cstdlib>
#include <vector>

namespace Showcase {
    class PacketProtocol {
    public:
        PacketProtocol(int header_size, int max_frame_size);
        int get_header_length();
        virtual int get_body_length();
        void incr_offset(int offset);
        void update_offset(int buffer_offset);
        char* get_buffer(int index);
        size_t reset_buffer();

    private:
        int max_frame_size;
        int header_size;
        int body_size;

        std::vector<char> buffer_;
        int buffer_offset_;
    };
}

#endif //ASIO_NETWORK_PACKETPROTOCOL_H
