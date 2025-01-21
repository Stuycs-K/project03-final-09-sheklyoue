[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Chat Room
Group ID: 17

## Team Name
omegle v2

### Team Members 
- Lawrence Shek
- Li Han You
       
## Project Description:
Our project is a simple chat server that allows up to 25 users to connect. The chat will display messages when people connect and disconnect and has a username list containing all of the present people in the chat.

## Video Link
[If you are using a personal email, use this link](https://drive.google.com/drive/folders/1vfc27V8ZlNeRzdYNplDj3GIUbNcXlUd-?usp=sharing)
[If you are using @nycstudents.net email, use this link](https://drive.google.com/drive/folders/16HFcCR08AvItJJArvncwyC_0vRGYYKGL?usp=sharing)

## Errors to Know (We explain errors in the video)
1. The user list only works when you are connected locally using 127.0.0.1 for some reason. When connected across different machines, the user list will work occasionally but most of the time it will print the correct list then print random characters over it.  
2. When multiple people connect at the same time and the first person doesn't input their name in time, everyone has to input their name before the chat works.
3. When you have a message ready to send in the message window, the chat will not update until you send that message (not able to send messages simultaneously).

## Instructions:

#### External Libraries Used:
- ncurses 


1. Once the repo is pulled, create a maximum of 25 terminals to connect to the chat. Please note each client terminal needs to be at least 140x40 (also don't resize the window when running the client otherwise it breaks because of ncurses' fixed window sizes).
2. Choose a terminal to act as the server and run:
```
make compile
./server
```
3. On all of the other terminals, run:
```
./client <hostname>
```

3.1. If you want to run locally, run:
```
./client 127.0.0.1
```
4. This will start the chat on the client side and follow the instructions printed
