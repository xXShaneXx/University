#include <iostream>
#include <array>
#include "bot.hpp"

void PrintBoard(const std::array<char, 25>& board) {
    std::cout << "   0 1 2 3 4\n";
    for (int row = 0; row < 5; ++row) {
        std::cout << row << "  ";
        for (int col = 0; col < 5; ++col) {
            char c = board[row * 5 + col];
            std::cout << (c == ' ' ? '.' : c) << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int main() {
    std::array<char, 25> board;
    board.fill(' ');

    Player current = Player::X;
    State result = State::NOT_FINISHED;

    while (true) {
        int move = Bot::FindBestMove(board, current);
        if (move == -1) break;

        board = Bot::Result(move, current, board);

        std::cout << "Player " << (current == Player::X ? 'X' : 'O') << " moves to (" 
                  << move / 5 << "," << move % 5 << "):\n";
        PrintBoard(board);

        result = Bot::Evaluate(board);
        if (result != State::NOT_FINISHED) break;

        current = (current == Player::X) ? Player::O : Player::X;
    }

    switch (result) {
        case State::WIN_X:
            std::cout << "X wins!\n"; break;
        case State::WIN_O:
            std::cout << "O wins!\n"; break;
        case State::DRAW:
            std::cout << "Draw!\n"; break;
        default:
            break;
    }

    return 0;
}
