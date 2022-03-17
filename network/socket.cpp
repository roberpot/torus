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
#include <shell.h>
#include <library/string.h>

#include <iostream>

using namespace Packets;
using namespace Packets::In;

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
    _client = new Client(this);
    _seed = 0;
    _seeded = false;

    _input_buffer = new uint8_t[toruscfg.buffer_size];
    memset(_input_buffer, 0, toruscfg.buffer_size);

    _input_buffer_tmp = new t_byte[toruscfg.buffer_size];
    memset(_input_buffer_tmp, 0, toruscfg.buffer_size);

    _output_buffer = new t_byte[toruscfg.buffer_size];
    memset(_output_buffer, 0, toruscfg.buffer_size);

    _output_encrypted_buffer = new uint8_t[toruscfg.buffer_size];
    memset(_output_encrypted_buffer, 0, toruscfg.buffer_size);

    if (_server_type == ConnectionType::CONNECTIONTYPE_GAMESERVER)
    {
        _connection_state = ConnectionState::CONNECTIONSTATE_CHARLIST;
    }

    if ((_connection_type == ConnectionType::CONNECTIONTYPE_CLIENT) &&
        (_server_type == ConnectionType::CONNECTIONTYPE_LOGINSERVER))
    {
        const udword_t packet_0xef_size = 21;

        receive(packet_0xef_size);
        set_connection_state(ConnectionState::CONNECTIONSTATE_LOGIN);
        LoginConnect* packet_login_connect = static_cast<LoginConnect*>(packet_factory(0x80));
        _current_in_packet = packet_login_connect;
        receive(packet_login_connect->length());        
        //_current_in_packet = nullptr;
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
        PacketIn* p = _packets_in_queue.front();
        delete p;
        _packets_in_queue.pop();
    }
    while (_packets_out_queue.empty() == false)
    {
        PacketOut* p = _packets_out_queue.front();
        delete p;
        _packets_out_queue.pop();
    }
    if (_client)
    {
        delete _client;
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
    return (read == 1);
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
    uword_t buffer_len = 0;
    if (receive_len > toruscfg.buffer_size)
    {
        receive_len = toruscfg.buffer_size;
    }

#ifdef _WINDOWS
    buffer_len = uword_t(recv(_socket, _input_buffer_tmp, receive_len, 0));
    if (buffer_len == SOCKET_ERROR)
    {
        TORUSSHELLECHO("Socket recv error: " << WSAGetLastError());
        return false;
    }

#endif // _WINDOWS
#ifdef __linux__
    buffer_len = (udword_t)recv(_socket, _input_buffer_tmp, receive_len, 0);
    if (buffer_len < 0)
    {
        TORUSSHELLECHO("Socket recv error: " << strerror(errno));
        return false;
    }
    else if(buffer_len == 0)
    {
        set_read_write_closed();
    }
#endif //__linux__

    memcpy(_input_buffer, _input_buffer_tmp, receive_len);

    if (buffer_len > 0 && buffer_len < 1024)
    {
        //TORUSSHELLECHO("recv done for a total bytes of " << buffer_len);
    }
    else
    {
        set_read_closed(true);
        return false;
    }


    t_ubyte first_byte = _input_buffer[0];
    if (_seeded == false)
    {
        // Apply some filters
        // check for new seed (sometimes it's received on its own).
        if (buffer_len == 1 && first_byte == 239)
        {
            return true;
        }
            // other ping data
        else if (buffer_len < 4 && first_byte != 160) // packet 0xa0 has 4 bytes, let's not skip it.
        {
            return true;
        }


        // Filters done, now handle some specific behaviours:
        // More than 4 bytes and fifth byte == new seed?
        if (t_ubyte(_input_buffer[4]) == 145)  // 0x91
        {
            //Seed beign received upon new connection, along with packet 0x91.
            int seed = int((unsigned char)(_input_buffer[0]) << 24 |
                           (unsigned char)(_input_buffer[1]) << 16 |
                           (unsigned char)(_input_buffer[2]) << 8 |
                           (unsigned char)(_input_buffer[3]));
            _seed = seed;
            _seeded = true;

            // Clean the current buffer and copy it again from the received buffer from position 4.
            memset(_input_buffer, 0, toruscfg.buffer_size);
            memcpy(_input_buffer, &_input_buffer_tmp[4], buffer_len);//Copy from pos 4 to the end of the buffer.
            buffer_len -= 4;    // Decrease the buffer len, since 4 bytes were removed.
        }
    }

    //TORUSSHELLECHO("Socket " << this << " receive data (0x" << hex(_input_buffer[0]) << ")[" << std::dec << buffer_len << "] = " << std::endl << hex_dump_buffer(_input_buffer, buffer_len));
    uword_t total_readed_bytes = 0;
    //auto vec_buffer = buffer_to_vector(_input_buffer_tmp, buffer_len);
    while (buffer_len > 0)
    {
        t_ubyte id = _input_buffer[0];
        if (_current_in_packet == nullptr)  // Try to retrieve an incomplete packet.
        {
            _current_in_packet = packet_factory(id);
            if (_current_in_packet)
            {
                //TORUSSHELLECHO("Creating new packet[" << hex(id) << "]: " << _current_in_packet);
            }
            else
            {
                TORUSSHELLECHO("Droping buffer for packet (0x" << hex(_input_buffer[0]) << ")[" << std::dec << buffer_len << "] = " << std::endl << hex_dump_buffer(_input_buffer, buffer_len));
                break;
            }

        }
        if (_current_in_packet)
        {
            //TORUSSHELLECHO("Writing data to packet: " << _current_in_packet);
            uword_t packet_len = _current_in_packet->length();
            uword_t bytes_to_read = buffer_len;
            if (bytes_to_read > packet_len)
            {
                bytes_to_read = packet_len;
            }

            uword_t readed_bytes = _current_in_packet->receive(_input_buffer, bytes_to_read);

            if (buffer_len - readed_bytes < 0)    // This happens when a packet is received along a chunk of the next one.
            {
                buffer_len = 0;
            }
            else
            {
                buffer_len -= readed_bytes;
            }
            if (_current_in_packet->is_complete())
            {
                //TORUSSHELLECHO("Storing packet << " << _current_in_packet << "(0x" << hex(_current_in_packet->packet_id()) << ") in _packets_in_queue");
                _packets_in_queue.push(_current_in_packet);
                _current_in_packet = nullptr;   // The full packet has been received, clean this pointer so the next data is attacked to another packet.
            }
            if (buffer_len > 0)
            {
                DEBUG_INFO("Recursive packet read");
                //auto vec_before = buffer_to_vector(_input_buffer, readed_bytes);
                memset(_input_buffer, 0, toruscfg.buffer_size);
                memcpy(_input_buffer, &_input_buffer_tmp[total_readed_bytes + readed_bytes], buffer_len);
                //auto vec_after = buffer_to_vector(_input_buffer, buffer_len);
                total_readed_bytes += readed_bytes;
            }
        }
    }
    memset(_input_buffer_tmp, 0, toruscfg.buffer_size);
    memset(_input_buffer, 0, toruscfg.buffer_size);
    return true;
}

void Socket::write(PacketOut* packet)
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

Socket* Socket::create_socket(sockaddr_in& sockin, ConnectionType server_type)
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
    s->_connection_info = sockin;
#endif //__linux__
    return s;
}

socket_t Socket::get_socket()
{
    return _socket;
}

bool Socket::client_pending(sockaddr_in& sockin) {
    ADDTOCALLSTACK();
#ifdef _WINDOWS
    UNREFERENCED_PARAMETER(sockin);
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
    _accepted_socket = accept(_socket, reinterpret_cast<struct sockaddr*>(&sockin), &len);
    return (_accepted_socket != -1);
#endif //__linux__
}

void Socket::bind(const t_byte* addr, uword_t port)
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
    return dword_t(_connection_info.sin_addr.s_addr);
#endif
}

void Socket::send_queued_packets()
{
    ADDTOCALLSTACK();
    while (!_packets_out_queue.empty())
    {
        PacketOut* out_packet = _packets_out_queue.front();
        if (!_is_write_closed)
        {
            //out_packet->print("Sending ");
            TORUSSHELLECHO("Socket " << this << " send(" << out_packet->length() << ") " << std::endl << hex_dump_buffer(out_packet->data(), out_packet->length()));
            udword_t out_len = 0;
            if (_server_type == ConnectionType::CONNECTIONTYPE_GAMESERVER)
            {
                out_len = _huffman.compress(_output_encrypted_buffer, out_packet->data(), toruscfg.buffer_size, out_packet->length());
                //TORUSSHELLECHO("Compressed data =  " << this << " send(" << out_len << ") " << std::endl << hex_dump_buffer(_output_encrypted_buffer, out_len));
                memcpy(_output_buffer, _output_encrypted_buffer, out_len);
            }
            else
            {
                out_len = out_packet->length();
                memcpy(_output_buffer, out_packet->data(), out_len);
            }
            if (out_len == 0)
            {
                TORUSSHELLECHO("Failed to compress, aborting the send of this packet.");
                //Failed, too much data?
            }
            else
            {
#ifdef _WINDOWS
                udword_t sent_len = send(_socket, _output_buffer, out_len, 0);
                if (sent_len == SOCKET_ERROR) {
                    THROW_ERROR(NetworkError, "Send failed with error: " << WSAGetLastError());
                }
                else if (sent_len != out_len) {
                    THROW_ERROR(NetworkError, "Send " << sent_len << " bytes, instead of " << out_len << " bytes.");
                }
#endif // _WINDOWS
#ifdef __linux__
                ssize_t sent_len = send(_socket, _output_buffer, out_len, 0);
                if (sent_len == -1) {
                    THROW_ERROR(NetworkError, "Send failed");
                }
                else if (sent_len != out_len) {
                    THROW_ERROR(NetworkError, "Send " << sent_len << " bytes, instead of " << out_len << " bytes.");
                }
#endif //__linux__
            }
            memset(_output_encrypted_buffer, 0, toruscfg.buffer_size);
            memset(_output_buffer, 0, toruscfg.buffer_size);
            //TODO: Encryption (after compression)
        }
        _packets_out_queue.pop();
        delete out_packet;
    }
}

void Socket::read_queued_packets()
{
    ADDTOCALLSTACK();
    while (!_packets_in_queue.empty())
    {
        PacketIn* in_packet = _packets_in_queue.front();
        if (_is_read_closed == false)
        {
            //TORUSSHELLECHO("Reading packet << " << in_packet << "(0x" << hex(in_packet->packet_id()) << ") from _packets_in_queue");
            in_packet->process(this);
        }
        _packets_in_queue.pop();
        delete in_packet;
    }
}

std::vector<uint8_t> Socket::buffer_to_vector(uint8_t* buffer, udword_t len)
{
    std::vector<uint8_t> vec;
    for (udword_t i = 0; i < len; ++i)
    {
        vec.push_back(buffer[i]);
    }
    return vec;
}



std::vector<uint8_t> Socket::buffer_to_vector(const t_byte* buffer, udword_t len)
{
    std::vector<uint8_t> vec;
    for (udword_t i = 0; i < len; ++i)
    {
        vec.push_back(uint8_t(buffer[i]));
    }
    return vec;
}

void Socket::clean_incoming_packets()
{
    ADDTOCALLSTACK();
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
