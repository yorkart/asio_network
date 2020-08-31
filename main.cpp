#define LOG4Z_FORMAT_INPUT_ENABLE

#include "libs/log4z.h"
#include "core/network/network_server.h"
#include <boost/thread.hpp>
#include <boost/functional.hpp>
#include <boost/bind.hpp>

extern volatile bool g_quit;

int main(int argc, char *argv[]) {
    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Start server ...");

    int server_port = 7654;
    int connet_num = 100;
    network_server srv(server_port, connet_num);

    boost::function<void()> fun = boost::bind(&network_server::run, &srv);
    boost::thread t(fun);

    while (!g_quit) {
        sleep(10);
    }

    srv.stop();
    t.join();

    LOGFMT_INFO(LOG4Z_MAIN_LOGGER_ID, "Stop server ...");

    return 0;
}