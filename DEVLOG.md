# Dev Log:

This document must be updated daily by EACH group member.

## Lawrence

### 2025-01-07 - Outline project files and functions
- Looked into *ncurses* library 

### 2025-01-08 - Discuss specifics on how project will be implemented 
- wrote function to read chat

### 2025-01-09 - Work on server/client communication
- user input sent back and forth between client and server
- tried to expand to multiple clients

### 2025-01-10 - Work on server connection with multiple clients
- working with just 2 clients (kind of broken) 
- decided to implement using sockets instead of pipes

### 2025-01-12 - Basic chat function working (worked together with eric)
- multiple clients can connect to the server 
- implemented sockets 
- server keeps track of all clients
- clients are able to input a name
- the chat is refreshed after every message sent 
- however, refreshing only happens after the client side sends a message
- need to look into ncurses 

### 2025-01-13 - Worked on setting up server connection between different machines
- cmd line args to indicate host to connect to
- explored ncurses functions (plan to use to handle simultaneous input from different users) 

### 2025-01-14 - Restructured communication between server/clients
- fixed minors bugs with closing server/client connections
- instead of the server writing to a chat file, server writes to each of the clients (who will each have their own individual chat files to read from)
  - if server is not local host, old version of having a single chat file wouldn't work
 
### 2025-01-15 - Debugging
- having issues with clients getting messages from server
- need to have each client update messages automatically, without blokcing
- need to do more debugging

## Eric

### 2025-01-08 - Organization and clear_terminal()
- Wrote the makefile to run a temp main.
- organized function headers and wrote descriptions
- wrote and tested clear_terminal

### 2025-01-09 - server.c
- attempted to get multiple clients to connect

### 2025-01-12 - Basic chat function working (worked together with lawrence)
- multiple clients can connect to the server 
- implemented sockets 
- server keeps track of all clients
- clients are able to input a name
- the chat is refreshed after every message sent 
- however, refreshing only happens after the client side sends a message
- need to look into ncurses 
