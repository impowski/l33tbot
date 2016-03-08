#include "twitch_irc.hpp"

using namespace std;
using namespace boost::asio::ip;

twitch_irc::twitch_irc(const string &server, const string &port, function<void()> on_connect)
    : server{server}, port{port}, on_connect{on_connect}, resolver{io_service}, socket{io_service}
        {

        }

twitch_irc::~twitch_irc()
{
    socket.close();
    io_service.stop();
}

void twitch_irc::connect()
{
    cout << __PRETTY_FUNCTION__ << "\n";
    cout << "Connecting to " << server << ":" << port << endl;
    tcp::resolver::query query(server, port);
    resolver.async_resolve(query,
                           bind(&twitch_irc::handle_resolve, this, _1, _2));
}

void twitch_irc::connect(const string &server, const string &port)
{
    this->server = server;
    this->port = port;
    connect();
}

void twitch_irc::write(const string &message)
{
    boost::asio::async_write(socket, boost::asio::buffer(message+"\r\n"),
                             bind(&twitch_irc::handle_write, this, _1));
}

void twitch_irc::read(const boost::system::error_code &error)
{
    boost::asio::async_read_until(socket, read_buffer, "\r\n",
                                  bind(&twitch_irc::handle_read, this, _1));
}

void twitch_irc::run()
{
    cout << __PRETTY_FUNCTION__ << "\n";
    connect();
    io_service.run();
}

void twitch_irc::close()
{
    socket.close();
    io_service.stop();
}

bool twitch_irc::alive() const
{
    return socket.is_open();
}

void twitch_irc::set_read_handler(const read_handler_type &handler)
{
    read_handler = handler;
}

void twitch_irc::set_write_handler(const write_handler_type &handler)
{
    write_handler = handler;
}

void twitch_irc::handle_resolve(const boost::system::error_code &error, tcp::resolver::iterator it)
{
    if (!error) {
        auto endpoint = *it;
        socket.async_connect(endpoint,
                             bind(&twitch_irc::handle_connect, this, _1, ++it));
    }
}

void twitch_irc::handle_connect(const boost::system::error_code &error, tcp::resolver::iterator it)
{
    if (!error) {
        on_connect();
        boost::asio::async_read_until(socket, read_buffer, "\r\n",
                                      bind(&twitch_irc::read, this, _1));
    } else if (it != tcp::resolver::iterator()) {
        socket.close();
        auto endpoint = *it;
        socket.async_connect(endpoint,
                             bind(&twitch_irc::handle_connect, this, _1, ++it));
    } else {
        cerr << error.message() << endl;
    }

}

void twitch_irc::handle_write(const boost::system::error_code &error)
{
    if (error) {
        cerr << error.message() << endl;
    }
}

void twitch_irc::handle_read(const boost::system::error_code &error)
{
    if (!error) {
        string line;
        while (getline(istream(&read_buffer), line)) {
            cout << line << endl;
            read_handler(line);
            if (boost::starts_with(line, "PING :")) {
                ostringstream oss;
                oss << "PONG :" << line.substr(6, line.size()) << "\r\n";
                boost::asio::async_write(socket, boost::asio::buffer(oss.str()),
                                         bind(&twitch_irc::handle_write, this, _1));
            }
        }
        boost::asio::async_read_until(socket, read_buffer, "\r\n",
                                      bind(&twitch_irc::read, this, _1));
    } else {
        close();
        cerr << error.message() << endl;
    }
}
