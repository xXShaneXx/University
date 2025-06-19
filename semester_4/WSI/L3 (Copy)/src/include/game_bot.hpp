#pragma once

#include "board.hpp"
#include <string>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>

class GameBot {
public:
    GameBot(const std::string& ip, const std::string& port, 
            const std::string& playerId, const std::string& nickname, int depth);
    ~GameBot();
    
    void run();
    
private:
    static constexpr int INF = 10000;
    
    std::string ip;
    std::string port;
    std::string playerId;
    std::string nickname;
    int depth;
    
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
    
    // AI functions
    bool isMovesLeft() const;
    bool hasLineOfLength(int length, int playerCheck) const;
    int evaluateHeuristic() const;
    int evaluate() const;
    int minMax(int depth, bool isMax, int alpha, int beta);

    int findBestMove();
};