#pragma once

#include <array>
#include <vector>
#include <utility>

class Board {
public:
    static constexpr int SIZE = 5;
    
    Board();
    
    void reset();
    void print() const;
    bool setMove(int move, int player);
    bool winCheck(int player) const;
    bool loseCheck(int player) const;
    
    const auto& getBoard() const { return board; }
    auto& getBoard() { return board; }
    
    using Position = std::pair<int, int>;
    using Line = std::vector<Position>;
    
    std::array<std::array<int, SIZE>, SIZE> board;
    
    static const std::vector<Line> winningLines;
    static const std::vector<Line> losingLines;
    
    bool checkLines(int player, const std::vector<Line>& lines) const;
};