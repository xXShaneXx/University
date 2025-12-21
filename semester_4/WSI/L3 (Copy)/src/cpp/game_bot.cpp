#include "game_bot.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <algorithm>

GameBot::GameBot(const std::string &ip, const std::string &port,
                 const std::string &playerId, const std::string &nickname, int depth)
    : ip(ip), port(port), playerId(playerId), nickname(nickname), depth(depth),
      serverSocket(-1), player(0), endGame(false)
{
}

GameBot::~GameBot()
{
    if (serverSocket != -1)
    {
        close(serverSocket);
    }
}

void GameBot::run()
{
    connectToServer();
    sendInitialMessage();
    processGame();
}

void GameBot::connectToServer()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        throw std::runtime_error("Unable to create socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        throw std::runtime_error("Unable to connect to server");
    }
}

void GameBot::sendInitialMessage()
{
    char serverMessage[16] = {0};
    if (recv(serverSocket, serverMessage, sizeof(serverMessage), 0) < 0)
    {
        throw std::runtime_error("Error while receiving server's message");
    }

    std::string playerMessage = playerId + " " + nickname;
    if (send(serverSocket, playerMessage.c_str(), playerMessage.size(), 0) < 0)
    {
        throw std::runtime_error("Unable to send message");
    }

    player = std::stoi(playerId);
    board.reset();
}

void GameBot::processGame()
{
    while (!endGame)
    {
        char serverMessage[16] = {0};
        if (recv(serverSocket, serverMessage, sizeof(serverMessage), 0) < 0)
        {
            throw std::runtime_error("Error while receiving server's message");
        }

        handleServerMessage(serverMessage);
    }
}

void GameBot::handleServerMessage(const std::string &message)
{
    int msg = std::stoi(message);
    int move = msg % 100;
    msg = msg / 100;

    if (move != 0)
    {
        board.setMove(move, 3 - player);
    }

    if (msg == 0 || msg == 6)
    {
        makeMove();
    }
    else
    {
        endGame = true;
        switch (msg)
        {
        case 1:
            std::cout << "You won.\n";
            break;
        case 2:
            std::cout << "You lost.\n";
            break;
        case 3:
            std::cout << "Draw.\n";
            break;
        case 4:
            std::cout << "You won. Opponent error.\n";
            break;
        case 5:
            std::cout << "You lost. Your error.\n";
            break;
        }
    }
}

void GameBot::makeMove()
{
    int move = findBestMove();
    std::cout << "Bot attempting move: " << move << std::endl; // Debug log
    board.setMove(move, player);

    std::string playerMessage = std::to_string(move);
    if (send(serverSocket, playerMessage.c_str(), playerMessage.size(), 0) < 0)
    {
        throw std::runtime_error("Unable to send message");
    }
}

bool GameBot::isMovesLeft() const
{
    for (const auto &row : board.getBoard())
    {
        for (int cell : row)
        {
            if (cell == 0)
                return true;
        }
    }
    return false;
}


int GameBot::evaluate() const {
    if (board.winCheck(1)) return INF;
    if (board.winCheck(2)) return -INF;
    if (board.loseCheck(1)) return -INF;
    if (board.loseCheck(2)) return INF;
    return evaluateHeuristic();
}


int GameBot::evaluateHeuristic() const {
    auto& b = board.getBoard();
    int score = 0;

    // Evaluate potential winning lines (4 in a row)
    for (const auto& line : Board::winningLines) {
        int playerCount = 0;
        int oppCount = 0;
        for (const auto& [i, j] : line) {
            if (b[i][j] == player) {
                playerCount++;
            } else if (b[i][j] == 3 - player) {
                oppCount++;
            }
        }

        // If a line has only bot's pieces and no opponent's, it's a potential win
        if (playerCount > 0 && oppCount == 0) {
            if (playerCount == 3) score += 1000; // Very strong threat of winning next turn
            else if (playerCount == 2) score += 100;
            else if (playerCount == 1) score += 10;
        }
        // If a line has only opponent's pieces and no bot's, it's a potential threat
        else if (oppCount > 0 && playerCount == 0) {
            if (oppCount == 3) score -= 1000; // Opponent has a very strong threat
            else if (oppCount == 2) score -= 100;
            else if (oppCount == 1) score -= 10;
        }
    }

    if (Board::SIZE % 2 != 0) { 
        int center_i = Board::SIZE / 2;
        int center_j = Board::SIZE / 2;
        if (b[center_i][center_j] == player) {
            score += 20; // Reward for controlling the center
        } else if (b[center_i][center_j] == 3 - player) {
            score -= 20; // Penalize if opponent controls the center
        }
    }

    return score;
}

int GameBot::minMax(int depth, bool isMax, int alpha, int beta)
{
    int score = evaluate();
    if (score == INF || score == -INF || depth == 0 || !isMovesLeft())
    {
        return score;
    }

    auto &b = board.getBoard();

    // Generate all possible moves and order them by proximity to center
    std::vector<std::tuple<int, int, int>> moves;
    int center = Board::SIZE / 2;
    for (int i = 0; i < Board::SIZE; i++) {
        for (int j = 0; j < Board::SIZE; j++) {
            if (b[i][j] == 0) {
                int dist = abs(i - center) + abs(j - center);
                moves.emplace_back(dist, i, j);
            }
        }
    }
    std::sort(moves.begin(), moves.end());

    if (isMax)
    {
        int best = -INF;
        for (auto& [dist, i, j] : moves)
        {
            b[i][j] = player;
            best = std::max(best, minMax(depth - 1, false, alpha, beta));
            b[i][j] = 0;
            alpha = std::max(alpha, best);
            if (beta <= alpha)
                break;
        }
        return best;
    }
    else
    {
        int best = INF;
        for (auto& [dist, i, j] : moves)
        {
            b[i][j] = 3 - player;
            best = std::min(best, minMax(depth - 1, true, alpha, beta));
            b[i][j] = 0;
            beta = std::min(beta, best);
            if (beta <= alpha)
                break;
        }
        return best;
    }
}


int GameBot::findBestMove() {
    auto &b = board.getBoard();
    int bestMove = -1;
    int bestValue = (player == 1) ? -INF : INF;

    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            if (b[i][j] == 0) {
                b[i][j] = player;
                int moveValue = minMax(depth - 1, player == 2, -INF, INF);
                std::cout << "Move: (" << i + 1 << ", " << j + 1 << "), Value: " << moveValue << std::endl;
                
                b[i][j] = 0;

                if ((player == 1 && moveValue > bestValue) ||
                    (player == 2 && moveValue < bestValue)) {
                    bestValue = moveValue;
                    bestMove = (i + 1) * 10 + (j + 1); // move encoding: row/col to move
                }
            }
        }
    }

    return bestMove;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <ip> <port> <player_id> <nickname> <depth>\n";
        return 1;
    }

    try
    {
        GameBot bot(argv[1], argv[2], argv[3], argv[4], std::stoi(argv[5]));
        bot.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}