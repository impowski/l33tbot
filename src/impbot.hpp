#ifndef IMPBOT_HPP
#define IMPBOT_HPP

#include "twitch_irc.hpp"

class impbot
{
public:
    impbot();
    virtual ~impbot();
    void auth(const std::string &auth);
    void nick(const std::string &nick);
    void join(const std::string &channel);
    void message(const std::string &receiver, const std::string &message);
    void loop();

    void add_read_handler(std::function<void (const std::string&)>);
    void read_handler(const std::string &message);
    void write_handler();
private:
    std::vector<std::function<void (const std::string&)> > read_handlers;
    twitch_irc irc;
};

#endif
