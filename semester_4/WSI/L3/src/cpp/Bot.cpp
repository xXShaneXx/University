#include "bot.hpp"
#include <array>
#include <vector>

bool Bot::IsMovesLeft(const std::array<char, 25>& board) {
    for (char cell : board) {
        if (cell == ' ') return true;
    }
    return false;
}

char Bot::check_line(const std::array<char, 25>& board, int x, int y, int dir, int len) {
    const int dx[] = {1, 0, 1, -1};  // kierunki: →, ↓, ↘, ↙
    const int dy[] = {0, 1, 1, 1};
    const int size = 5;

    char first = board[y * size + x];
    if (first == ' ') return ' ';  // jeśli pierwsze pole puste, to nie ma linii

    for (int i = 1; i < len; ++i) {
        int nx = x + dx[dir] * i;
        int ny = y + dy[dir] * i;
        
        // Jeśli wyjdziemy poza planszę lub pole jest puste lub inny symbol → nie ma linii
        if (nx < 0 || nx >= size || ny < 0 || ny >= size || board[ny * size + nx] != first) {
            return ' ';
        }
    }
    return first;  // jeśli wszystkie pola są takie same i nieprzerwane
}

bool Bot::check_potential_line(int x, int y, int dir, int len, const std::array<char, 25>& board, char symbol) {
    const int dx[] = {1, 0, 1, -1};
    const int dy[] = {0, 1, 1, 1};
    const int size = 5;

    for (int i = 0; i < len; ++i) {
        int nx = x + dx[dir] * i;
        int ny = y + dy[dir] * i;
        if (nx < 0 || nx >= size || ny < 0 || ny >= size) return false;
        char cell = board[ny * size + nx];
        if (cell != symbol && cell != ' ') return false;
    }
    return true;
}

int Bot::safe_three_count(char symbol, const std::array<char, 25>& board) {
    const int dx[] = {1, 0, 1, -1};
    const int dy[] = {0, 1, 1, 1};
    int safe_count = 0;

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            for (int dir = 0; dir < 4; ++dir) {
                int sx = x;
                int sy = y;
                int own = 0, empty = 0, blocked = 0;

                for (int i = 0; i < 4; ++i) {
                    int nx = sx + i * dx[dir];
                    int ny = sy + i * dy[dir];
                    if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) {
                        blocked = 1;
                        break;
                    }

                    char c = board[ny * 5 + nx];
                    if (c == symbol) own++;
                    else if (c == ' ') empty++;
                    else blocked = 1;
                }

                if (!blocked && own == 3 && empty == 1) {
                    safe_count++;
                }
            }
        }
    }

    return safe_count;
}

State Bot::Evaluate(const std::array<char, 25>& board) {
    const int size = 5;
    bool x_has_three = false;
    bool o_has_three = false;

    // Sprawdź wszystkie możliwe linie 4 symboli (nieprzerwane)
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            for (int dir = 0; dir < 4; ++dir) {
                char line4 = check_line(board, x, y, dir, 4);
                if (line4 == 'X') return State::WIN_X;
                if (line4 == 'O') return State::WIN_O;

                // Sprawdź linie 3 symboli (tylko jeśli nie są częścią linii 4)
                char line3 = check_line(board, x, y, dir, 3);
                if (line3 == 'X' && check_line(board, x, y, dir, 4) != 'X') x_has_three = true;
                if (line3 == 'O' && check_line(board, x, y, dir, 4) != 'O') o_has_three = true;
            }
        }
    }

    // Jeśli ktoś ma linię 3 (nie będącą częścią linii 4), przegrywa
    if (x_has_three) return State::WIN_O;
    if (o_has_three) return State::WIN_X;

    if (!IsMovesLeft(board)) return State::DRAW;
    return State::NOT_FINISHED;
}

std::vector<int> Bot::Actions(const std::array<char, 25>& board) {
    std::vector<int> moves;
    for (int i = 0; i < 25; ++i) {
        if (board[i] == ' ') {
            moves.push_back(i);
        }
    }
    return moves;
}

std::array<char, 25> Bot::Result(int index, Player player, const std::array<char, 25>& board) {
    std::array<char, 25> new_board = board;
    char symbol = (player == Player::X) ? 'X' : 'O';
    new_board[index] = symbol;
    return new_board;
}

int Bot::HeuristicEval(const std::array<char, 25>& board, int depth) {
    State result = Evaluate(board);
    if (result == State::WIN_X) return 10000 - 10 * depth;
    if (result == State::WIN_O) return -10000 + 10 * depth;
    if (result == State::DRAW) return 0;

    int score = 0;

    auto count_potential_lines = [&](char symbol, int len) {
        int count = 0;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                for (int dir = 0; dir < 4; ++dir) {
                    if (check_potential_line(x, y, dir, len, board, symbol)) {
                        count++;
                    }
                }
            }
        }
        return count;
    };

    // Offensive
    score += 30 * count_potential_lines('X', 2);
    score += 300 * safe_three_count('X', board);

    // Defensive
    score -= 30 * count_potential_lines('O', 2);
    score -= 300 * safe_three_count('O', board);

    // Immediate threats/opportunities
    for (int move : Actions(board)) {
        auto new_board = Result(move, Player::O, board);
        if (Evaluate(new_board) == State::WIN_O) {
            score -= 5000; // for allowing O to win next move
        }
        new_board = Result(move, Player::X, board);
        if (Evaluate(new_board) == State::WIN_X) {
            score += 5000; //  for X winning next move
        }
    }

    // Center Control
    const std::vector<int> center_indices = {6, 7, 8, 11, 12, 13, 16, 17, 18};
    for (int i : center_indices) {
        if (board[i] == 'X') score += 30;
        if (board[i] == 'O') score -= 30;
    }

    return score;
}

int Bot::MinMax(std::array<char, 25> board, int depth, bool isMax, int alpha, int beta) {
    State result = Evaluate(board);
    if (result != State::NOT_FINISHED || depth == 0) {
        return HeuristicEval(board, depth);
    }

    if (isMax) {
        int best = -10000;
        for (int move : Actions(board)) {
            auto next_board = Result(move, Player::X, board);
            int val = MinMax(next_board, depth - 1, false, alpha, beta);
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int best = 10000;
        for (int move : Actions(board)) {
            auto next_board = Result(move, Player::O, board);
            int val = MinMax(next_board, depth - 1, true, alpha, beta);
            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

int Bot::FindBestMove(const std::array<char, 25>& board, Player player) {
    char symbol = (player == Player::X) ? 'X' : 'O';
    
    int bestMove = -1;
    int bestValue = (player == Player::X) ? -10000 : 10000;
    bool maximizing = (player == Player::X);
    int depth = 6;  

    auto moves = Actions(board);
    for (int move : moves) {
        auto new_board = Result(move, player, board);
        
        //Avoid moves that create a losing three-in-a-row
        State immediate_result = Evaluate(new_board);
        // Skip moves that create an immediate loss
        if ((player == Player::X && immediate_result == State::WIN_O) || 
            (player == Player::O && immediate_result == State::WIN_X)) {
            continue;
        }
        
        int moveValue = MinMax(new_board, depth, !maximizing, -10000, 10000);

        // Immediate win check
        if ((player == Player::X && immediate_result == State::WIN_X) || 
            (player == Player::O && immediate_result == State::WIN_O)) {
            return move;
        }

        if ((maximizing && moveValue > bestValue) || 
            (!maximizing && moveValue < bestValue)) {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    
    // If we couldn't find a good move (all moves lead to loss), just pick the first available
    if (bestMove == -1 && !moves.empty()) {
        return moves[0];
    }
    
    return bestMove;
}