#include "NPuzzle.hpp"
#include <algorithm> 
#include <random>

NPuzzle::NPuzzle(size_t size, const std::vector<uint8_t>& tiles) 
    : size_(size), board_(tiles) {
    for (size_t i = 0; i < board_.size(); ++i) {
        if (board_[i] == 0) {
            empty_pos_ = i;
            break;
        }
    }
}

NPuzzle::NPuzzle(size_t size)
{
    generate(size);
}

NPuzzle::NPuzzle(const NPuzzle& other)
    : size_(other.size_), board_(other.board_),
    empty_pos_(other.empty_pos_), g(other.g) 
{}

void NPuzzle::generate(size_t size) {
    size_ = size;
    board_.resize(size * size);

    // Wypełnij wartości od 1 do n*n - 1
    for (size_t i = 0; i < board_.size() - 1; ++i) {
        board_[i] = i + 1;
    }
    board_.back() = 0; // pusty kafelek na końcu

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(board_.begin(), board_.end() - 1, g); // mieszamy bez 0

    empty_pos_ = board_.size() - 1; // 0 zawsze na końcu
}

void NPuzzle::set_heuristic(std::unique_ptr<Heuristic> heuristic) {
    heuristic_ = std::move(heuristic);
}


size_t NPuzzle::get_size() const { return size_; }
size_t NPuzzle::get_empty_pos() const { return empty_pos_; }
const std::vector<uint8_t>& NPuzzle::get_board() const { return board_; }

bool NPuzzle::is_solution() const {
    for (size_t i = 0; i < board_.size() - 1; ++i) {
        if (board_[i] != i + 1) return false;
    }
    return board_.back() == 0;
}

std::vector<NPuzzle> NPuzzle::generate_moves() const {
    std::vector<NPuzzle> moves;
    const size_t row = empty_pos_ / size_;
    const size_t col = empty_pos_ % size_;
    
    if (row > 0) moves.push_back(create_swapped(empty_pos_ - size_)); // Up
    if (row < size_ - 1) moves.push_back(create_swapped(empty_pos_ + size_)); // Down
    if (col > 0) moves.push_back(create_swapped(empty_pos_ - 1)); // Left
    if (col < size_ - 1) moves.push_back(create_swapped(empty_pos_ + 1)); // Right
    
    return moves;
}

bool NPuzzle::operator==(const NPuzzle& other) const {
    return board_ == other.board_;
}

NPuzzle NPuzzle::create_swapped(size_t new_pos) const {
    NPuzzle new_state(*this);
    std::swap(new_state.board_[empty_pos_], new_state.board_[new_pos]);
    new_state.empty_pos_ = new_pos;
    return new_state;
}

void NPuzzle::increase_moves() {
    g++;
}

int NPuzzle::total_cost() {
    if(heuristic_)
    {
        return g + (*heuristic_)(board_);
    }
}

namespace std {
    size_t hash<NPuzzle>::operator()(const NPuzzle& puzzle) const {
        size_t hash = 0;
        for (auto tile : puzzle.get_board()) {
            hash = hash * 31 + tile;
        }
        return hash;
    }
}