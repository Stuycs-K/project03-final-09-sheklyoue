[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Chat Room
Group ID: 17

## Team Name
omegle v2

### Team Members 
- Lawrence Shek
- Li Han You
       
## Project Description:
Our project is a simple group chat that allows for multiple people to join and keeps track of their names. The chat will display messages when people connect and disconnect and has a username list containing all of the present people in the chat.

## Video Link
  

## Errors to Know (We explain the errors in the Video)
1. The user list only works when you are connected locally using 127.0.0.1 for some reason. When connected across different machines, the user list will work occasionally but most of the time it will print the correct list then print random characters over it. 
2. When multiple people connect at the same time and the first person doesn't input their name in time, everyone has to input their name before the chat works.

## Instructions:
1. Once the repo is pulled, create a maximum of 25 terminals to connect to the chat. 
2. Choose a terminal to act as the server and run:
```
make compile
./server
```
3. On all of the other terminals, run:
```
./client (address)
```

3.1. If you want to run locally, run:
```
./client 127.0.0.1
```
4. This will start the chat on the client side and follow the instructions printed
