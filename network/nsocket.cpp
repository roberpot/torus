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


#include <network/nsocket.h>


NSocket::NSocket(socket_t s, ConnectionType connection_type) :
    _current_in_packet(nullptr),
    _is_read_closed(false),
    _is_write_closed(false),
    _socket(s),
    _connection_state(ConnectionState::CONNECTIONSTATE_NONE),
    _connection_type(connection_type)
{
    ADDTOCALLSTACK();

    memset(&_connection_info, 0, sizeof(sockaddr_in));
}

NSocket::~NSocket()
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

bool NSocket::data_ready()
 {
    ADDTOCALLSTACK();
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(_socket, &readSet);
    timeval timeout;
    timeout.tv_sec = 0;  // Zero timeout (poll)
    timeout.tv_usec = 0;
    return (select((int)_socket, &readSet, NULL, NULL, &timeout) == 1);
 }

void NSocket::receive()
{
    ADDTOCALLSTACK();
    // First check if we rewinded.
    const udword_t buffer_read = 1024;
    udword_t buffer_len = 0;

    t_byte *buffer = new t_byte[buffer_read];
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
    buffer_len = recv(_socket, buffer, buffer_read, 0);
    if (buffer_len == SOCKET_ERROR)
    {
        TORUSSHELLECHO("Socket recv error: " << WSAGetLastError());
        return;
    }

#endif // _WINDOWS
#ifdef __linux__
    buffer_len = (udword_t)recv(_socket, &buffer[init], len_remaining, 0);
    if (buffer_len == 0)
    {
        TORUSSHELLECHO("Socket recv error: " << strerror(errno));
        return;
    }
#endif //__linux__

    if (buffer_len > 0 && buffer_len < 1024)
    {
        //TORUSSHELLECHO("recv done for a total bytes of " << buffer_len);
        TORUSSHELLECHO("Network receive(0x" << hex(buffer[0]) << ")[ " << buffer_len << " ] = " << std::endl << hex_dump_buffer(buffer, buffer_len));
    }
    if (_current_in_packet == nullptr)  // Try to retrieve an incomplete packet.
    {
        _current_in_packet = packet_factory(buffer[0]);
    }

    if (_current_in_packet != nullptr)
    {
        _current_in_packet->receive(buffer, buffer_len);
        if (_current_in_packet->is_complete())
        {
            _packets_in_queue.push(_current_in_packet);
            _current_in_packet = nullptr;   // The full packet has been received, clean this pointer so the next data is attacked to another packet.
        }
    }

    delete[] buffer;
}

 void NSocket::write(PacketOut * packet)
 {
    _packets_out_queue.push(packet);
 }

 ConnectionState NSocket::get_connection_state()
 {
     return _connection_state;
 }

 void NSocket::set_connection_state(ConnectionState connection_state)
 {
    _connection_state = connection_state;
 }

 ConnectionType NSocket::get_connection_type()
 {
     return _connection_type;
 }

 NSocket* NSocket::create_socket()
 {
     ADDTOCALLSTACK();
     NSocket* s;
#ifdef _WINDOWS
     s = new NSocket(accept(_socket, 0, 0));
#endif //_WINDOWS
#ifdef __linux__
     s = new NSocket(_accepted_socket);
#endif //__linux__
     return s;
 }

 void NSocket::bind(const t_byte* addr, word_t port)
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
         THROW_ERROR(NetworkError, "bind failed with error: " << WSAGetLastError() << ". Can not bind " << addr << ":" << port);
     }
     status = listen(_socket, SOMAXCONN);
     if (status == SOCKET_ERROR) {
         closesocket(_socket);
         THROW_ERROR(NetworkError, "listen failed with error: " << WSAGetLastError());
     }
#endif // _WINDOWS
#ifdef __linux__
     if (status < 0) {
         THROW_ERROR(NetworkError, "bind failed with error: " << strerror(errno) << ". Can not bind " << addr << ":" << port);
     }
     status = listen(_socket, SOMAXCONN);
     if (status < 0) {
         THROW_ERROR(NetworkError, "listen failed with error: " << strerror(errno));
     }
#endif //__linux__

     socklen_t _addr_size = sizeof(_connection_info);
     getpeername(_socket, (sockaddr*)&_connection_info, &_addr_size);
 }

 const t_byte* NSocket::get_ip_str()
 {
     ADDTOCALLSTACK();
     return inet_ntoa(_connection_info.sin_addr);;
 }

 const dword_t NSocket::get_ip()
 {
     return _connection_info.sin_addr.S_un.S_addr;
 }

 void NSocket::send_packets()
 {
     while (!_packets_out_queue.empty())
     {
         PacketOut* out_packet = _packets_out_queue.front();
         _packets_out_queue.pop();
         //out_packet->print("Sending ");
         //TORUSSHELLECHO("Network send(" << out_packet->length() << ") " << std::endl << hex_dump_buffer(out_packet->dumps(), out_packet->length()));

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
         ssize_t data_sended = send(_socket, out_packet->dumps(), out_packet->length(), 0);
         if (data_sended == -1) {
             THROW_ERROR(NetworkError, "Send failed");
         }
         else if (data_sended != out_packet->length()) {
             THROW_ERROR(NetworkError, "Send " << data_sended << " bytes, instead of " << out_packet->length() << " bytes.");
         }
#endif //__linux__
         delete out_packet;
     }
 }
