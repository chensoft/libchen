/**
 * Created by Jian Chen
 * @since  2016.04.10
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#ifdef CHEN_OS_UNIX

#include "base_socket_unix.hpp"
#include <chen/net/so/so_error.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace chen;
using namespace chen::so;

// -----------------------------------------------------------------------------
// socket
socket::socket()
{
}

socket::~socket()
{
    if (this->_impl)
        this->close();
}

// close
void socket::close()
{
    if (!this->_impl)
        return;

    ::close(this->_impl->_socket);
    this->_impl.reset();
}

void socket::shutdown(Shutdown flag)
{
    if (!this->_impl)
        return;

    if (flag == Shutdown::Read)
        ::shutdown(this->_impl->_socket, SHUT_RD);
    else if (flag == Shutdown::Write)
        ::shutdown(this->_impl->_socket, SHUT_WR);
    else if (flag == Shutdown::Both)
        ::shutdown(this->_impl->_socket, SHUT_RDWR);
}

// remote
std::string socket::remoteAddr() const
{
    if (!this->_impl)
        return "";

    struct sockaddr_in in;
    socklen_t len = sizeof(in);

    ::memset(&in, 0, sizeof(in));

    if (::getpeername(this->_impl->_socket, (struct sockaddr*)&in, &len) == -1)
        return "";
    else
        return ::inet_ntoa(in.sin_addr);
}

std::uint16_t socket::remotePort() const
{
    if (!this->_impl)
        return 0;

    struct sockaddr_in in;
    socklen_t len = sizeof(in);

    ::memset(&in, 0, sizeof(in));

    if (::getpeername(this->_impl->_socket, (struct sockaddr*)&in, &len) == -1)
        return 0;
    else
        return ntohs(in.sin_port);
}

// local
std::string socket::localAddr() const
{
    if (!this->_impl)
        return "";

    struct sockaddr_in in;
    socklen_t len = sizeof(in);

    ::memset(&in, 0, sizeof(in));

    if (::getsockname(this->_impl->_socket, (struct sockaddr*)&in, &len) == -1)
        return "";
    else
        return ::inet_ntoa(in.sin_addr);
}

std::uint16_t socket::localPort() const
{
    if (!this->_impl)
        return 0;

    struct sockaddr_in in;
    socklen_t len = sizeof(in);

    ::memset(&in, 0, sizeof(in));

    if (::getsockname(this->_impl->_socket, (struct sockaddr*)&in, &len) == -1)
        return 0;
    else
        return ntohs(in.sin_port);
}

#endif  // CHEN_OS_UNIX