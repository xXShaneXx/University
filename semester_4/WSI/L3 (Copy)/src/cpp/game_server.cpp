#include "game_server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>

GameServer::GameServer(const std::string& ip, const std::string& port)
    : ip(ip), port(port), serverSocket(-1), playerSockets({-1, -1}),
      firstPlayer(0), currentPlayer(1), moveCount(0), 
      gameOver(false), correctMove(true) {
}

GameServer::~GameServer() {
    for (auto socket : playerSockets) {
        if (socket != -1) {
            close(socket);
        }
    }
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

void GameServer::run() {
    setupServer();
    acceptConnections();
    initializeGame();
    processGame();
}

void GameServer::setupServer() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Error while creating socket");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Couldn't bind to the port");
    }

    if (listen(serverSocket, 1) < 0) {
        throw std::runtime_error("Error while listening");
    }
}

void GameServer::acceptConnections() {
    socklen_t playerSize = sizeof(playerAddrs[0]);
    playerSockets[0] = accept(serverSocket, (struct sockaddr*)&playerAddrs[0], &playerSize);
    if (playerSockets[0] < 0) {
        throw std::runtime_error("Can't accept player 1");
    }

    playerSize = sizeof(playerAddrs[1]);
    playerSockets[1] = accept(serverSocket, (struct sockaddr*)&playerAddrs[1], &playerSize);
    if (playerSockets[1] < 0) {
        throw std::runtime_error("Can't accept player 2");
    }
}

void GameServer::initializeGame() {
    // Player 1 recognition
    sendMessage(0, "700");
    std::string message = receiveMessage(0);
    
    int player1Choice;
    std::istringstream iss(message);
    iss >> player1Choice >> nicknames[0];
    
    firstPlayer = (player1Choice == 1) ? 0 : 1;
    
    // Player 2 recognition
    sendMessage(1, "700");
    message = receiveMessage(1);
    
    int player2Choice;
    iss.clear();
    iss.str(message);
    iss >> player2Choice >> nicknames[1];
    
    if (player1Choice == player2Choice) {
        throw std::runtime_error("No player diversity");
    }
    
    sendMessage(firstPlayer, "600");
    board.reset();
    currentPlayer = 1;
    moveCount = 0;
    gameOver = false;
    correctMove = true;
}

void GameServer::processGame() {
    int currentPlayerIndex = firstPlayer;
    
    while (moveCount < 25 && !gameOver && correctMove) {
        moveCount++;
        
        std::string message = receiveMessage(currentPlayerIndex);
        int move = std::stoi(message);
        
        handleMove(move);
        
        currentPlayerIndex = 1 - currentPlayerIndex;
        currentPlayer = 3 - currentPlayer;
    }
    
    if (gameOver) {
        endGame(moveCount == 25 ? 0 : (currentPlayer == 1 ? 2 : 1));
    }
}

void GameServer::sendMessage(int playerIndex, const std::string& message) {
    if (send(playerSockets[playerIndex], message.c_str(), message.size(), 0) < 0) {
        throw std::runtime_error("Can't send message to player");
    }
}

std::string GameServer::receiveMessage(int playerIndex) {
    char buffer[16] = {0};
    if (recv(playerSockets[playerIndex], buffer, sizeof(buffer), 0) < 0) {
        throw std::runtime_error("Couldn't receive message from player");
    }
    return std::string(buffer);
}

void GameServer::handleMove(int move) {
    correctMove = board.setMove(move, currentPlayer);
    bool isWin = false;
    bool isLose = false;
    
    if (correctMove) {
        isWin = board.winCheck(currentPlayer);
        if (!isWin) {
            isLose = board.loseCheck(currentPlayer);
        }
    }
    
    int msg = 0;
    int endMsg = 0;
    
    if (!correctMove) {
        msg = 400;
        endMsg = 500;
        gameOver = true;
    } else if (isWin) {
        msg = 200 + move;
        endMsg = 100;
        gameOver = true;
    } else if (isLose) {
        msg = 100 + move;
        endMsg = 200;
        gameOver = true;
    } else if (moveCount == 25) {
        msg = 300 + move;
        endMsg = 300;
        gameOver = true;
    } else {
        msg = move;
    }
    
    sendMessage(1 - (currentPlayer - 1), std::to_string(msg));
    board.print();
    
    if (gameOver) {
        sendMessage(currentPlayer - 1, std::to_string(endMsg));
    }
}

void GameServer::endGame(int result) {
    std::cout << nicknames[firstPlayer] << " " << nicknames[1 - firstPlayer] << " " << result << "\n";
    std::cout << "End.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
        return 1;
    }

    try {
        GameServer server(argv[1], argv[2]);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}