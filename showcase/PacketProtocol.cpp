#include "PacketProtocol.h"

namespace Showcase {
    uint32_t bytes2Int(const uint8_t* bytes){
        uint32_t value=0;
        value = ((bytes[0] & 0xff)<<24)|
                ((bytes[1] & 0xff)<<16)|
                ((bytes[2] & 0xff)<<8) |
                ( bytes[3] & 0xff);

        return value;
    }

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
        if (this->body_size == 0) {
            this->body_size = bytes2Int(reinterpret_cast<const uint8_t *>(&this->buffer_[5]));
            this->reset_buffer();
        }

        return this->body_size;
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

    size_t PacketProtocol::reset_buffer() {
        int capacity = this->get_header_length() + this->get_body_length();
        if (this->buffer_.capacity() < capacity && capacity <= this->max_frame_size) {
            this->buffer_.resize(capacity);
            return 1;
        }
        return 0;
    }


}
