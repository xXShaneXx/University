#include "bot.hpp"
#include <array>
#include <iostream>

// Helper to print test results
void print_result(const std::string& test_name, bool passed) {
    std::cout << test_name << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
}

// Test: X wins with 4 in a row horizontally
bool test_x_wins_horizontal() {
    std::array<char, 25> board = {
        ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ',
        'X', 'X', 'X', 'X', ' ', // 4 in row for X
        'O', 'O', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' '
    };
    
    return Bot::Evaluate(board) == State::WIN_X;
}

// Test: O wins with 4 in a row vertically 
bool test_o_wins_vertical() {
    std::array<char, 25> board = {
        ' ', ' ', 'O', ' ', ' ',
        ' ', ' ', 'O', ' ', ' ',
        ' ', ' ', 'O', ' ', ' ',
        ' ', ' ', 'O', ' ', ' ',
        ' ', ' ', ' ', ' ', ' '
    };
    
    return Bot::Evaluate(board) == State::WIN_O;
}

// Test: X loses with 3 in a row (not part of 4)
bool test_x_loses_with_three() {
    std::array<char, 25> board = {
        ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ',
        'X', 'X', 'X', ' ', ' ', // 3 in row for X
        'O', 'O', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' '
    };
    
    return Bot::Evaluate(board) == State::WIN_O;
}

// Test: O loses with 3 in a row diagonal
bool test_o_loses_with_three_diagonal() {
    std::array<char, 25> board = {
        'O', ' ', ' ', ' ', ' ',
        ' ', 'O', ' ', ' ', ' ',
        ' ', ' ', 'O', ' ', ' ', // O diagonal (3)
        ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' '
    };
    
    return Bot::Evaluate(board) == State::WIN_X;
}

// Main test function
int main() {
    int passed = 0;
    int total = 0;
    
    // Run tests and track results
    bool t1 = test_x_wins_horizontal(); total++; if(t1) passed++;
    print_result("X wins horizontal", t1);
    
    bool t2 = test_o_wins_vertical(); total++; if(t2) passed++;
    print_result("O wins vertical", t2);
    
    bool t3 = test_x_loses_with_three(); total++; if(t3) passed++;
    print_result("X loses with three", t3);
    
    bool t4 = test_o_loses_with_three_diagonal(); total++; if(t4) passed++;
    print_result("O loses with three diagonal", t4);
    
    std::cout << "Passed " << passed << "/" << total << " tests" << std::endl;
    
    return (passed == total) ? 0 : 1;
}