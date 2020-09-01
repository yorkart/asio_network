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
        common/InputStream.cpp
        common/InputStream.h
        common/io_streambuf.h
        common/net_stream.cpp
        common/net_stream.h
        common/OutputStream.cpp
        common/OutputStream.h
#        common/singleton.h
#        core/network/api_network.cpp
        core/network/AsyncAcceptor.cpp
        core/network/AsyncAcceptor.h
        core/network/NetworkServer.cpp
        core/network/NetworkServer.h
        core/network/SessionHandler.cpp
        core/network/SessionHandler.h
        include/error_code.h
        libs/log4z.cpp
        libs/log4z.h
        protocol/packet_protocol.cpp
        protocol/packet_protocol.h
        protocol/protocol_network.h
        showcase/SessionHandler.cpp showcase/SessionHandler.h showcase/NetworkServer.cpp showcase/NetworkServer.h showcase/async_main.cpp showcase/AsyncAcceptor.cpp showcase/AsyncAcceptor.h showcase/PacketProtocol.cpp showcase/PacketProtocol.h)

target_link_libraries( asio_network ${Boost_LIBRARIES} )