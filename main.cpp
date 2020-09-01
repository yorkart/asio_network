#define LOG4Z_FORMAT_INPUT_ENABLE

#include "libs/log4z.h"
#include "core/network/NetworkServer.h"
#include "showcase/NetworkServer.h"
#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/bind.hpp>

int main(int argc, char *argv[]) {
    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Start server ...");

    int server_port = 36366;
    int connet_num = 100;


    Showcase::NetworkServer serve(server_port);
    serve.run();

    NetworkServer srv(server_port, connet_num);

    boost::function<void()> fun = boost::bind(&NetworkServer::run, &srv);
    boost::thread t(fun);

    while (true) {
        sleep(100);
    }

    srv.stop();
    t.join();

    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Stop server ...");

    return 0;
}