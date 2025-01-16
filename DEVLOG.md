# Dev Log:

This document must be updated daily by EACH group member.

## Lawrence

### 2024-01-02 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-03 - Brief description
Expanded description including how much time was spent on task.

### 2025-01-07 - Outline project files and functions
- Looked into *ncurses* library 

### 2025-01-08 - Discuss specifics on how project will be implemented 
- wrote function to read chat

### 2025-01-09 - Work on server/client communication
- user input sent back and forth between client and server
- tried to expand to multiple clients

### 2025-01-12 - Basic chat function working (worked together with eric)
- multiple clients can connect to the server 
- the chat is refreshed after every message sent 
- however, refreshing only happens after the client side sends a message
- need to look into ncurses 

## Eric

### 2024-01-02 - Brief description
Expanded description including how much time was spent on task.

### 2024-01-03 - Brief description
Expanded description including how much time was spent on task.

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

### 2025-01-13 - ncurses
- read up on ncurses
- did some testing with multi-windows 
- planned out what the chat will look like

### 2025-01-14
- worked on creating the basic format of chat
- making it take in messages
- tried to create a user list
- having trouble with sytnax

### 2025-01-14
- the chat format works
- chat is updating and messages are being sent
- however, chat is not being constantly updated
- user list also not working 
- need to find a way to send all client info over to each client 