# Socket_Client_Server
A multithreaded server program that accepts a message from a client program, writes the message to a text file, and then returns a message to the client. The function which writes to the text file is protected by a POSIX semaphore in order to allow safe concurrent connections to the server from multiple clients.
