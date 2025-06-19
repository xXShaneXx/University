#ifndef BOT_HPP
#define BOT_HPP

#include <vector>

enum class State: char {
    WIN_X = 1,
    DRAW = 0,
    WIN_O = -1,
    NOT_FINISHED
};

enum class Player: char {
    X = 'X',
    O = 'O'
};

struct Bot {
    //returns Heuristic Evaluation
    static int HeuristicEval(const std::array<char, 25>& board, int depth); 

    // Returns the best move for the current player (X or O)
    static int MinMax(std::array<char, 25> board, int depth, bool isMax, int alpha, int beta);

    // Evaluates the board and returns the State (WIN_X, WIN_O, DRAW)
    static State Evaluate(const std::array<char, 25>& board);

    // Returns a vector of possible move indices (empty cells)
    static std::vector<int> Actions(const std::array<char, 25>& board);

    //Return board after the move
    static std::array<char, 25> Result(int index, Player player, const std::array<char, 25>& board);

    // Checks if there are moves left on the board
    static bool IsMovesLeft(const std::array<char, 25>& board);

    // Returns the best move index for the current player
    static int FindBestMove(const std::array<char, 25>& board, Player player);

    //
    static char check_line(const std::array<char, 25>& board, int x, int y, int dir, int len);

    //
    static bool check_potential_line(int x, int y, int dir, int len, const std::array<char, 25>& board, char symbol);

    //
    static int safe_three_count(char symbol, const std::array<char, 25>& board);
};

#endif // BOT_HPP