#include "game_client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

GameClient::GameClient(const std::string& ip, const std::string& port, 
                       const std::string& playerId, const std::string& nickname)
    : ip(ip), port(port), playerId(playerId), nickname(nickname),
      serverSocket(-1), player(0), endGame(false) {
}

GameClient::~GameClient() {
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

void GameClient::run() {
    connectToServer();
    sendInitialMessage();
    processGame();
}

void GameClient::connectToServer() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Unable to create socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Unable to connect to server");
    }
}

void GameClient::sendInitialMessage() {
    char serverMessage[16] = {0};
    if (recv(serverSocket, serverMessage, sizeof(serverMessage), 0) < 0) {
        throw std::runtime_error("Error while receiving server's message");
    }
    std::cout << "Server message: " << serverMessage << "\n";

    std::string playerMessage = playerId + " " + nickname;
    if (send(serverSocket, playerMessage.c_str(), playerMessage.size(), 0) < 0) {
        throw std::runtime_error("Unable to send message");
    }

    player = std::stoi(playerId);
    board.reset();
}

void GameClient::processGame() {
    while (!endGame) {
        char serverMessage[16] = {0};
        if (recv(serverSocket, serverMessage, sizeof(serverMessage), 0) < 0) {
            throw std::runtime_error("Error while receiving server's message");
        }
        std::cout << "Server message: " << serverMessage << "\n";

        handleServerMessage(serverMessage);
    }
}

void GameClient::handleServerMessage(const std::string& message) {
    int msg = std::stoi(message);
    int move = msg % 100;
    msg = msg / 100;

    if (move != 0) {
        board.setMove(move, 3 - player);
        board.print();
    }

    if (msg == 0 || msg == 6) {
        makeMove();
    } else {
        endGame = true;
        switch (msg) {
            case 1: std::cout << "You won.\n"; break;
            case 2: std::cout << "You lost.\n"; break;
            case 3: std::cout << "Draw.\n"; break;
            case 4: std::cout << "You won. Opponent error.\n"; break;
            case 5: std::cout << "You lost. Your error.\n"; break;
        }
    }
}

void GameClient::makeMove() {
    std::cout << "Your move: ";
    std::string input;
    std::getline(std::cin, input);
    
    int move = std::stoi(input);
    board.setMove(move, player);
    board.print();
    
    std::string playerMessage = std::to_string(move);
    if (send(serverSocket, playerMessage.c_str(), playerMessage.size(), 0) < 0) {
        throw std::runtime_error("Unable to send message");
    }
    std::cout << "Player message: " << playerMessage << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port> <player_id> <nickname>\n";
        return 1;
    }

    try {
        GameClient client(argv[1], argv[2], argv[3], argv[4]);
        client.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}