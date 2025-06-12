#pragma once

#include "board.hpp"
#include <string>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>

class GameClient {
public:
    GameClient(const std::string& ip, const std::string& port, 
               const std::string& playerId, const std::string& nickname);
    ~GameClient();
    
    void run();
    
private:
    std::string ip;
    std::string port;
    std::string playerId;
    std::string nickname;
    
    int serverSocket;
    sockaddr_in serverAddr;
    Board board;
    int player;
    bool endGame;
    
    void connectToServer();
    void sendInitialMessage();
    void processGame();
    void handleServerMessage(const std::string& message);
    void makeMove();
};