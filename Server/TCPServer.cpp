
/* 
 * File:   TCPServer.cpp
 * Author: czimbortibor
 * 
 * Created on December 16, 2016, 8:17 PM
 */

#include "TCPServer.h"

TCPServer::TCPServer(const char* address, int port) : address(address) {
    this->port = port;
    usersPtr = std::make_shared<std::list<std::unique_ptr<ClientThread>>>();
    
    /** initializing mutex and condition variables */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

TCPServer::~TCPServer() {
    if (address != NULL) {
        delete address;
        address = NULL;
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void TCPServer::initServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket < 0) {
        errorMsg = "error while opening the server socket"; 
        error(errorMsg.c_str());
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(address);
    serverAddr.sin_port = htons(port);

    int res = bind(listenSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    if (res < 0) {
        errorMsg = "error on binding";
        error(errorMsg.c_str());
    }
}

void TCPServer::listenServer() {
    std::cout << "Listening for clients...\n";
    int res = listen(listenSocket, 1);
    if (res < 0) {
        errorMsg = "error while listening on socket";
        error(errorMsg.c_str());
    }
    
    std::cout << "Accepting client...\n";
    socklen_t addrSize = sizeof(serverAddr);
    acceptSocket = accept(listenSocket, NULL, NULL);
    if (acceptSocket < 0) {
        errorMsg = "error while creating the accepting socket";
        error(errorMsg.c_str());
    }
}

void TCPServer::startServer() {
    initServer();
    while(1<2) {
        // creates a new socket for the connecting client
        listenServer();
        
        std::unique_ptr<ClientThread> clientThread(new ClientThread(*this, usersPtr, acceptSocket));
        std::cout << "Creating a new thread for the client...\n";
        clientThread->start();
        // move the ownership of the unique_ptr to the list and insert it into the list
        usersPtr->push_back(std::move(clientThread));
    }
}

// TODO: remove client
void TCPServer::removeClient(const std::string& username) {
   /* std::cout << "nr of users: " << usersPtr->size() << "\n";
    auto list = usersPtr.get();
    std::list<std::unique_ptr<ClientThread>>::const_iterator iterator;
    for (iterator = list->begin(); iterator != list->end(); ++iterator) {
        std::string tmp = (**iterator).getUsername();
        if (tmp.compare(username) == 0) {
            break;
        }
    }
    usersPtr->erase(iterator);
    std::cout << "nr of users after delete: " << usersPtr->size() << "\n"; */
}

std::vector<std::string> TCPServer::getOnlineUsers() {
    std::vector<std::string> onlineUsers;
  
    auto list = usersPtr.get();
    std::list<std::unique_ptr<ClientThread>>::const_iterator iterator;
    for (iterator = list->begin(); iterator != list->end(); ++iterator) {
        onlineUsers.push_back((**iterator).getUsername());
    }
    
    return onlineUsers;
}