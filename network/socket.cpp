/*
 * This file is part of Torus.
 * Torus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Torus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with Torus. If not, see <http://www.gnu.org/licenses/>.
 */


#include <network/socket.h>
#include <core/config.h>    //Include after socket, this includes windows.h which gives conflicts with winsock2
#include <game/client.h>

#include <iostream>

Socket::Socket(socket_t s, ConnectionType connection_type, ConnectionType server_type) :
    _socket(s),
    _connection_type(connection_type),
    _server_type(server_type)
{
    ADDTOCALLSTACK();
    _init();
    memset(&_connection_info, 0, sizeof(sockaddr_in));
}

Socket::Socket(ConnectionType connection_type) :
    _connection_type(connection_type)
{
    ADDTOCALLSTACK();
    _init();

#ifdef _WINDOWS
    //_socket = INVALID_SOCKET;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == INVALID_SOCKET) {
        THROW_ERROR(NetworkError, "Create socket failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (_socket < 0) {
        throw NetworkError("ERROR opening socket");
    }
#endif //__linux__

    memset(&_connection_info, 0, sizeof(sockaddr_in));
}

void Socket::_init()
{
    _current_in_packet = nullptr;
    _is_read_closed = false;
    _is_write_closed = false;
    _connection_state = ConnectionState::CONNECTIONSTATE_NONE;
    _client = nullptr;
    _seed = 0;

    _buffer = new t_byte[BUFFER_SIZE];
    memset(_buffer, 0, BUFFER_SIZE);

    if (_server_type == ConnectionType::CONNECTIONTYPE_GAMESERVER)
    {
        _connection_state = ConnectionState::CONNECTIONSTATE_CHARLIST;
    }

    if ( (_connection_type == ConnectionType::CONNECTIONTYPE_CLIENT) &&
            (_server_type == ConnectionType::CONNECTIONTYPE_LOGINSERVER))
    {
        const udword_t packet_0xef_size = 21;
        const udword_t packet_0x80_size = 62;

        //PacketIn *packet_0xef = packet_factory(0xef);
        //_current_in_packet = packet_0xef;
        receive(packet_0xef_size);
        set_connection_state(ConnectionState::CONNECTIONSTATE_LOGIN);

        PacketIn *packet_in = packet_factory(0x80);
        PacketIn_0x80 *packet_0x80 = static_cast<PacketIn_0x80*>(packet_in);
        _current_in_packet = packet_0x80;
        packet_0x80->set_from_loginserver();
        receive(packet_0x80_size);
        _current_in_packet = nullptr;
        PacketOut_0xa8* packet = new PacketOut_0xa8();
        packet->set_data(this);
        packet->send(this);
        set_connection_state(ConnectionState::CONNECTIONSTATE_SERVERLIST);
    }
}

Socket::~Socket()
{
    ADDTOCALLSTACK();
    if (_current_in_packet != nullptr)
    {
        delete _current_in_packet;
    }
    while (_packets_in_queue.empty() == false)
    {
        PacketIn *p = _packets_in_queue.front();
        delete p;
        _packets_in_queue.pop();
    }
    while (_packets_out_queue.empty() == false)
    {
        PacketOut* p = _packets_out_queue.front();
        delete p;
        _packets_out_queue.pop();
    }

#ifdef _WINDOWS
    closesocket(_socket);
#endif // _WINDOWS
#ifdef __linux__
    close(_socket);
#endif //__linux__

}

bool Socket::data_ready(fd_set& readSet)
 {
    ADDTOCALLSTACK();
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    int read = select((int)_socket, &readSet, NULL, NULL, &timeout);
    if (read < 0)
    {
        set_read_write_closed(true);
    }
    return ( read == 1 );
 }

bool Socket::is_read_closed()
{
    return _is_read_closed;
}

void Socket::set_read_closed(bool state)
{
    _is_read_closed = state;
}

bool Socket::is_write_closed()
{
    return _is_write_closed;
}

void Socket::set_write_closed(bool state)
{
    _is_write_closed = state;
}

void Socket::set_read_write_closed(bool state)
{
    _is_read_closed = state;
    _is_write_closed = state;
}


bool Socket::receive(udword_t receive_len)
{
    ADDTOCALLSTACK();
    // First check if we rewinded.
    udword_t buffer_len = 0;
    if (receive_len > BUFFER_SIZE)
    {
        receive_len = BUFFER_SIZE;
    }

    /*if (rewinded_len > 0) {
        TORUSSHELLECHO("rewinding");
        if (len_remaining < rewinded_len) {
            init = len_remaining;
        }
        else {
            init = rewinded_len;
        }
        memcpy(buffer, rewinded, sizeof(t_byte) * init);
        rewinded_len -= init;
        len_remaining -= init;
        _cur_pos -= rewinded_len;
        if (rewinded_len == 0) {
            delete rewinded;
        }
        else {
            memmove(rewinded, &rewinded[init], sizeof(t_byte) * rewinded_len);
        }
        if (_cur_pos < 0) // Should never happen?
        {
            _cur_pos = 0;
        }

    }*/
#ifdef _WINDOWS
    buffer_len = recv(_socket, _buffer, receive_len, 0);
    if (buffer_len == SOCKET_ERROR)
    {
        TORUSSHELLECHO("Socket recv error: " << WSAGetLastError());
        return false;
    }

#endif // _WINDOWS
#ifdef __linux__
    buffer_len = (udword_t)recv(_socket, _buffer, receive_len, 0);
    if (buffer_len == 0)
    {
        TORUSSHELLECHO("Socket recv error: " << strerror(errno));
        return false;
    }
#endif //__linux__

    if (buffer_len > 0 && buffer_len < 1024)
    {
        //TORUSSHELLECHO("recv done for a total bytes of " << buffer_len);
        TORUSSHELLECHO("Socket " << this << " receive(0x" << hex(_buffer[0]) << ")[" << std::dec << buffer_len << "] = " << std::endl << hex_dump_buffer(_buffer, buffer_len));
    }
    else
    {
        set_read_closed(true);
    }

    if (_current_in_packet != nullptr)
    {
        /*if (_connection_state == ConnectionState::CONNECTIONSTATE_CHARLIST)
        {
            if (_current_in_packet->packet_id() == 0)
            {
                delete _current_in_packet;
                _current_in_packet = nullptr;
            }
        }*/
    }

    if (_current_in_packet == nullptr)  // Try to retrieve an incomplete packet.
    {
        t_ubyte id = _buffer[0];
        _current_in_packet = packet_factory(id);
    }

    if (_current_in_packet != nullptr)
    {
        _current_in_packet->receive(_buffer, buffer_len);
        if (_current_in_packet->is_complete())
        {
            TORUSSHELLECHO("Pushing packet << " << _current_in_packet << "(0x" << hex(_current_in_packet->packet_id()) << ") to _packets_in_queue");
            _packets_in_queue.push(_current_in_packet);
            _current_in_packet = nullptr;   // The full packet has been received, clean this pointer so the next data is attacked to another packet.
        }
    }

    return true;
}

void Socket::write(PacketOut * packet)
{
    _packets_out_queue.push(packet);
}

ConnectionState Socket::get_connection_state()
{
    return _connection_state;
}

void Socket::set_server_type(ConnectionType type)
{
    _server_type = type;
}

ConnectionType Socket::get_server_type()
{
    return _server_type;
}

void Socket::set_connection_state(ConnectionState connection_state)
{
    _connection_state = connection_state;
}

ConnectionType Socket::get_connection_type()
{
    return _connection_type;
}

Socket* Socket::create_socket(sockaddr_in &sockin, ConnectionType server_type)
{
    ADDTOCALLSTACK();
    Socket* s;
#ifdef _WINDOWS
    s = new Socket(accept(_socket, reinterpret_cast<struct sockaddr*>(&sockin), 0),
        ConnectionType::CONNECTIONTYPE_CLIENT, server_type);
#endif //_WINDOWS
#ifdef __linux__
    s = new Socket(_accepted_socket);
    s->_connection_info.sin_family = AF_INET;
    s->_connection_info = sock_in;
#endif //__linux__
    return s;
}

socket_t Socket::get_socket()
{
    return _socket;
}

bool Socket::client_pending(sockaddr_in &sockin) {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    return (select((int)_socket, &readSet, NULL, NULL, &timeout) == 1);
#endif //_WINDOWS
#ifdef __linux__
    socklen_t len = sizeof(sockin);
    _accepted_socket = accept(_socket, reinterpret_cast<struct sockaddr *>(&sockin), &len);
    return (_accepted_socket != -1);
#endif //__linux__
}

void Socket::bind(const t_byte* addr, word_t port)
{
    ADDTOCALLSTACK();
    DEBUG_INFO("Binding " << addr << ":" << port);
    dword_t status;
    _connection_info.sin_family = AF_INET;
    _connection_info.sin_port = htons(port);
    _connection_info.sin_addr.s_addr = inet_addr(addr);
    status = ::bind(_socket, (sockaddr*)&_connection_info, sizeof(sockaddr));

#ifdef _WINDOWS
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        int err = WSAGetLastError();
        THROW_ERROR(NetworkError, "bind failed with error: " << err << ". Can not bind " << addr << ":" << port);
    }
    status = listen(_socket, SOMAXCONN);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        int err = WSAGetLastError();
        THROW_ERROR(NetworkError, "listen failed with error: " << err);
    }
#endif // _WINDOWS
#ifdef __linux__
    if (status < 0) {
        std::string err(strerror(errno));
        THROW_ERROR(NetworkError, "bind failed with error: " << err << ". Can not bind " << addr << ":" << port);
    }
    status = listen(_socket, SOMAXCONN);
    if (status < 0) {
        THROW_ERROR(NetworkError, "listen failed with error: " << strerror(errno));
    }
#endif //__linux__

    socklen_t _addr_size = sizeof(_connection_info);
    getpeername(_socket, (sockaddr*)&_connection_info, &_addr_size);
}

const t_byte* Socket::get_ip_str()
{
    ADDTOCALLSTACK();
    return inet_ntoa(_connection_info.sin_addr);;
}

dword_t Socket::get_ip()
{
#ifdef _WINDOWS
    return _connection_info.sin_addr.S_un.S_addr;
#endif
#ifdef __linux
    return _connection_info.sin_addr.s_addr;
#endif
}

void Socket::send_queued_packets()
{
    while (!_packets_out_queue.empty())
    {
        PacketOut* out_packet = _packets_out_queue.front();
        if (_is_write_closed == false)
        {
            //out_packet->print("Sending ");
            TORUSSHELLECHO("Network send(" << out_packet->length() << ") " << std::endl << hex_dump_buffer(out_packet->data(), out_packet->length()));

#ifdef _WINDOWS
            udword_t data_sended = send(_socket, out_packet->data(), out_packet->length(), 0);
            if (data_sended == SOCKET_ERROR) {
                THROW_ERROR(NetworkError, "Send failed with error: " << WSAGetLastError());
            }
            else if (data_sended != out_packet->length()) {
                THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << out_packet->length() << " bytes.");
            }
#endif // _WINDOWS
#ifdef __linux__
            ssize_t data_sended = send(_socket, out_packet->data(), out_packet->length(), 0);
            if (data_sended == -1) {
                THROW_ERROR(NetworkError, "Send failed");
            }
            else if (data_sended != out_packet->length()) {
                THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << out_packet->length() << " bytes.");
            }
#endif //__linux__
        }
        _packets_out_queue.pop();
        delete out_packet;
        break;
    }
}

void Socket::read_queued_packets()
{
    while (!_packets_in_queue.empty())
    {
        PacketIn* in_packet = _packets_in_queue.front();
        if (_is_read_closed == false)
        {
            TORUSSHELLECHO("Reading packet << " << in_packet << "(0x" << hex(in_packet->packet_id()) << ") from _packets_in_queue");
            in_packet->process(this);
            //TORUSSHELLECHO("Processed");
        }
        _packets_in_queue.pop();
        delete in_packet;
    }
}
void Socket::clean_incoming_packets()
{
    while (!_packets_in_queue.empty())
    {
        PacketIn* in_packet = _packets_in_queue.front();
        _packets_in_queue.pop();
        delete in_packet;
    }
    if (_current_in_packet != nullptr)
    {
        delete _current_in_packet;
        _current_in_packet = nullptr;
    }
}

Client* Socket::get_client()
{
    return _client;
}

void Socket::set_client(Client* client)
{  
    _client = client;
}

void Socket::shutdown()
{
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    set_read_write_closed();
    udword_t status = ::shutdown(_socket, SD_SEND);
    if (status == SOCKET_ERROR) {
        closesocket(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << WSAGetLastError());
    }
#endif // _WINDOWS
#ifdef __linux__
    dword_t status = ::shutdown(_socket, SHUT_WR);
    if (status == -1) {
        close(_socket);
        THROW_ERROR(NetworkError, "shutdown failed with error: " << strerror(errno));
    }
#endif //__linux__
}

void Socket::set_seed(udword_t seed)
{
    _seed = seed;
}

udword_t Socket::get_seed()
{
    return _seed;
}
