#Network quick-start guide
Network handles connections (accept, data management and disconnects) over sockets of the Socket class.

#Socket guide
Socket is a custom implementation of OS' socket classes (SOCKET for windows, int for linux), it handles clients' connections and data send/recieval in form of Packets

#Packet guide
A packet is a representation of it's data in form of a std::vector<t_byte>, each packet has it's own child class which is relegated to handle the data process and writing for it's own specific purposes