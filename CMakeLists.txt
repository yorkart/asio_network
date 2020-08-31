cmake_minimum_required(VERSION 3.16)
project(asio_network)

set(CMAKE_CXX_STANDARD 11)

set(Boost_INCLUDE_DIR /usr/local/opt/icu4c/include)
set(Boost_LIBRARY_DIR /usr/local/opt/icu4c/lib)

find_package(Boost COMPONENTS system filesystem REQUIRED thread)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

#add_subdirectory(api)

add_executable(asio_network main.cpp
        api/dllmain.cpp
        api/rtdb_client.cpp
        api/rtdb_client.h
        api/rtdb_client_impl.cpp
        api/rtdb_client_impl.h
        common/input_stream.cpp
        common/input_stream.h
        common/io_streambuf.h
        common/net_stream.cpp
        common/net_stream.h
        common/output_stream.cpp
        common/output_stream.h
        common/singleton.h
        core/network/api_network.cpp
        core/network/async_acceptor.cpp
        core/network/async_acceptor.h
        core/network/network_server.cpp
        core/network/network_server.h
        core/network/session_handler.cpp
        core/network/session_handler.h
        include/error_code.h
        libs/app.cpp
        libs/app.h
        libs/config.h
        libs/daemon.h
        libs/file.h
        libs/log4z.cpp
        libs/log4z.h
        protocol/packet_protocol.cpp
        protocol/packet_protocol.h
        protocol/protocol_network.h)

target_link_libraries( asio_network ${Boost_LIBRARIES} )