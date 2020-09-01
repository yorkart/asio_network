#include "PacketProtocol.h"

namespace Showcase {
    PacketProtocol::PacketProtocol(int header_size, int max_frame_size)
            : header_size(header_size), body_size(0), max_frame_size(max_frame_size), buffer_offset_(0) {
        if (this->buffer_.capacity() < header_size) {
            this->buffer_.resize(header_size);
        }
    }

    int PacketProtocol::get_header_length() {
        return this->header_size;
    }

    int PacketProtocol::get_body_length() {
        if (this->body_size > 0) {
            return this->body_size;
        }
        return 0;
    }

    void PacketProtocol::update_offset(int buffer_offset) {
        this->buffer_offset_ = buffer_offset;
    }

    char* PacketProtocol::get_buffer(int index) {
        return &this->buffer_[index];
    }

    void PacketProtocol::incr_offset(int offset) {
        this->buffer_offset_ += offset;
    }

    size_t PacketProtocol::reset_buffer(int capacity) {
        if (this->buffer_.capacity() < capacity && capacity <= this->max_frame_size) {
            this->buffer_.resize(capacity);
            return 1;
        }
        return 0;
    }
}
