# WhatsProg - Client-Server Application

WhatsProg is a client-server application developed in C++ with the aim of exchanging text messages between users on different machines. The server connects to all user clients and forwards messages to the correct recipients. The application utilizes threads and TCP sockets for communication between client and server, using port 23456.

Before using WhatsProg, every user must register with the server using a unique login and password. The login must be between 6 and 12 characters long, and the password must also be between 6 and 12 characters. Both the login and password are case-sensitive.

The recipient must be a previously registered user; otherwise, the message is rejected. After sending the message, the server sends notifications to the sender when the message is received by them and when it is transmitted to the recipient. Messages destined for a connected user should be transmitted immediately. If the user is disconnected, the message should be stored in a message buffer.

## Compilation on Windows:

In the compiler options, enable static linking, meaning that the necessary parts of the system libraries will be integrated into your executable code:
- Static **libgcc**
- Static **libstdc++**
- Static **linking**

Note that, for the MySocket library to work, it is necessary to link with the **Ws2_32** library.

## Compilation on Linux:

To compile on Linux, simply uncomment the indicated fields in the code.

## Data Persistence:

When the server program terminates or in case of inactivity, it saves the information to a file so that, when launched again, it is in the same previous state.

## User Interface:

Depending on the color customization of the operating system, some parts of the interface may be hidden, requiring adjustments to an appropriate color.
 
Overall, this project involves the implementation of a server, a client, and a QT graphical interface for the client. The inspiration for this project is the WhatsApp application.

---
This README was automatically generated from the provided project specifications.