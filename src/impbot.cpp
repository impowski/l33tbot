#include "impbot.hpp"

using namespace std;

impbot::impbot()
    : irc("irc.twitch.tv", "6667", [this](){
            this->auth("oauth:");
            this->nick("L33T_Bot");
            this->join("#hustle_man");
        })
{
    irc.set_write_handler([this](){ this->write_handler(); });
    irc.set_read_handler([this](const std::string &m){
            this->read_handler(m);
        });
}

impbot::~impbot()
{

}

void impbot::add_read_handler(std::function<void (const std::string &)> func)
{
    read_handlers.push_back(func);
}

void impbot::read_handler(const std::string &message)
{
    for (auto func : read_handlers)
        func(message);
}

void impbot::write_handler()
{
    string line;
    while (irc.alive()) {
        istringstream iss(line);
        message("#hustle_man", iss.str());
    }
}

void impbot::auth(const std::string &auth)
{
    irc.write("PASS " + auth);
}

void impbot::message(const std::string &receiver, const std::string &message)
{
    irc.write("PRIVMSG " + receiver + " :" + message);
}

void impbot::join(const std::string &channel)
{
    irc.write("JOIN " + channel);
}

void impbot::nick(const string& nick)
{
    irc.write("NICK " + nick);
}

void impbot::loop()
{
    irc.run();
}
