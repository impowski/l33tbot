#include <iostream>
#include <boost/asio.hpp>
#include "impbot.hpp"

using namespace std;
using namespace boost::asio::ip;

int main(int argc, char *argv[])
{
    try {
        impbot bot;

        bot.add_read_handler([&bot](const std::string &m){
                istringstream iss(m);
                string from, type, to, body;
                iss >> from >> type >> to >> body;
                if (boost::starts_with(body, ":!time")) {
                    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
                    bot.message(to, ctime(&now));
                }
            });

        bot.loop();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
