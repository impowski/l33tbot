#ifndef TWITCH_API_HPP
#define TWITCH_API_HPP

#include <string>
#include <iostream>

class twitch_api
{
public:
    twitch_api();
    virtual ~twitch_api();
    void get_user(const std::string &user);
private:
    std::string api_user;
};

#endif
