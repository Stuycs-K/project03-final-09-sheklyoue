# Final Project Proposal

## Group Members:

Lawrence Shek, Li Han You
       
# Intentions:

To create a simple chat server that allows multiple users to log on with a username and send messages to everyone else connected to the server. Some extra features we will implement are private chats, private chat rooms, a list of all chat members. 
    
# Intended usage:
Have users communicate with other users in the server. Also allow users to privately message each other and create and use group chats. 

# Technical Design  
The server will make use of processes (forking), pipes (maybe sockets if time permits) to send messages back and forth between users, shared memory to keep track of the messages and all the users connected, and and signals for users to disconnect from the server.

The data structures involved will probably be pretty simple, using arrays to keep track of all the users and chat history.
    
# Intended pacing:

#### 1/7-1/9
Set up project structure, working communication between server and single user.

#### 1/10-1/13
Implement multiple users connect to server and send messages back and forth.

#### 1/14-1/17
Build interface for users to view all other users connected to server, and implement private messaging between users. 

#### 1/19-1/21
Small additional features like keeping track of message history between users. 
