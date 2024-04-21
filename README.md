# ICS_Major_Project
# Readme

## Description
This code is a simple chat application implemented in C using the GTK library for the user interface and socket programming for communication between clients. It allows users to add contacts, establish connections with them, and exchange messages in real-time.
Download the zip file and extract it. Open it in any code editor or terminal , run make command in it. Then run the exectable 
named main.
A chat window gets opened.

## Requirements
- C compiler with support for GTK and pthread libraries.
- GTK development files installed.
- Basic understanding of socket programming.

## How to Use
1. Compile the code using a C compiler that supports GTK and pthread libraries.
2. Run the compiled executable.
3. Use the main window to navigate between adding users, chatting, and quitting.
4. In the "Add User" window, enter the name and IP address of the contact you want to add, then click "Add Contact".
5. Click on the "Chat" button to initiate a chat. Enter the receiver's name and click "Connect to User" to establish a connection.
6. Type your message in the message entry box and click "Done" to send it.
7. To view your contacts, click on the "Show Contacts" button.
8. To quit the chat window, click on the "Quit this Chat" button.

## Features
- **Add User**: Users can add contacts by providing their name and IP address.
- **Chat**: Users can initiate chats with their contacts. The chat window allows users to send and receive messages.
- **Show Contacts**: Users can view their list of contacts, including their names and IP addresses.
- **Quit Chat**: Users can close the chat window and return to the main window.
- **Connect to User**: Users can establish a connection with a selected contact and start chatting.
- **Done Button**: Sends the message typed in the message entry box.

## Files
- **b23cs1058_b23cs1014_b23me1053_b23ci1020_socket_client.h**: Header file containing function declarations and structures for the client-side.
- **b23cs1058_b23cs1014_b23me1053_b23ci1020_socket_server.h**: Header file containing function declarations and structures for the server-side.
- **b23cs1058_b23cs1014_b23me1053_b23ci1020_socket_client.c**: Source code file for the client-side implementation.
- **b23cs1058_b23cs1014_b23me1053_b23ci1020_socket_server.c**: Source code file for the server-side implementation.
- **b23cs1058_b23cs1014_b23me1053_b23ci1020_gui.c**: Main source code file containing the main function and GTK UI implementation.

## Dependencies
- GTK Library
- pthread Library
- SQLite3 Library

## Contributors
- Rishi Kaneria, Devesh Bhardwaj, Kiran Patil, Ravish Kumar.
