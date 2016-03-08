

#ifndef TWITCH_IRC_CONNECT_HPP
#define TWITCH_IRC_CONNECT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <exception>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>

using read_handler_type = std::function<void (const std::string&)>;
using write_handler_type = std::function<void (void)>;

class twitch_irc
{
public:
    twitch_irc(const std::string &server, const std::string &port, const std::function<void()> on_connect);
    virtual ~twitch_irc();
    void connect();
    void connect(const std::string &server, const std::string &port);
    void write(const std::string &message);
    void read(const boost::system::error_code &error);

    void run();
    void close();
    bool alive() const;

    void set_read_handler(const read_handler_type &handler);
    void set_write_handler(const write_handler_type &handler);
private:
    void handle_resolve(const boost::system::error_code &error,
                        boost::asio::ip::tcp::resolver::iterator it);
    void handle_connect(const boost::system::error_code &error,
                        boost::asio::ip::tcp::resolver::iterator it);
    void handle_write(const boost::system::error_code &error);
    void handle_read(const boost::system::error_code &error);

    std::string server;
    std::string port;
    std::function<void()> on_connect;

    boost::asio::streambuf read_buffer;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;

    read_handler_type read_handler;
    write_handler_type write_handler;
};

#endif
