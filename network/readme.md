#Network quick-start guide
Network handles connections (accept, data management and disconnects) over sockets of the Socket class.

#Socket guide
Socket is a custom implementation of OS' socket classes (SOCKET for windows, int for linux), it handles clients' connections and data send/recieval in form of Packets

#Packet guide
A packet is a representation of it's data in form of a std::vector<t_byte>, each packet has it's own child class which is relegated to handle the data process and writing for it's own specific purposes.


###Receiving packets:

The server reads the data and automatically tries to generate a packet for it, to tell Torus there is a packet for the received data it must be added inside the switch placed in Packet * packet_factory( ) adding as case match the packet's id and creating an object of that packet, eg:

    switch(t){
		case 0xa0:
			p = new Packet_0xa0();
			break;
		...
		...
	}

After creating the object, loads(socket) will be called on the packet to read the received data into the virtual method:

    if (p != nullptr)
    {
        p->loads(&s);
    }

###Sending packets:
To send a packet just create a new object of it and let the Socket write it, eg:

	Packet_0xa8 * packet = new Packet_0xa8();

This packet does it's job in the ctor, if anything else must be done, do it. After all it's data has been filled, it's time to send it:

	s->write_packet(packet);

The Socket delete it automatically, there's no need to manage it's memory.

>*s is the pointer to the Socket, depending on where the code is being executed it must be retrieved in a way or other.

#Login Sequence (Unencrypted clients)
1) When a client is connecting it sends a packet 0xef with the client seed and client version, with it the server stores the seed and cliver and must find the client keys to find the crypt hash (tho we decided to skip encryption this packet must be received and processed anyway).

2) With the last process done we have enough data to handle the connection and the client is waiting for confirmation so we send packet 0xa8, which contains the server's list.

3) The client has now the server's list, when one server is selected the client sends packet 0x80 with the account's information, which must here be checked for validity/etc, after all checks have been done we must send back to the client a packet 0x82 containing the connection response code, being 255 if the login attempt was correct.
