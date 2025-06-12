#pragma once

#include "board.hpp"
#include <string>
#include <array>
#include <sys/socket.h>
#include <netinet/in.h>

class GameServer {
public:
    GameServer(const std::string& ip, const std::string& port);
    ~GameServer();
    
    void run();
    
private:
    std::string ip;
    std::string port;
    
    int serverSocket;
    std::array<int, 2> playerSockets;
    std::array<sockaddr_in, 2> playerAddrs;
    std::array<std::string, 2> nicknames;
    
    Board board;
    int firstPlayer;
    int currentPlayer;
    int moveCount;
    bool gameOver;
    bool correctMove;
    
    void setupServer();
    void acceptConnections();
    void initializeGame();
    void processGame();
    void sendMessage(int playerIndex, const std::string& message);
    std::string receiveMessage(int playerIndex);
    void handleMove(int move);
    void endGame(int result);
};