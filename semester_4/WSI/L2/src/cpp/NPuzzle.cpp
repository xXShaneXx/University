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

NPuzzle::NPuzzle()
    : size_(0), board_(), empty_pos_(0), g(0), heuristic_(nullptr) {}

void NPuzzle::generate(size_t size) {
    size_ = size;
    board_.resize(size * size);

    // Fill values from 1 to n*n - 1
    for (size_t i = 0; i < board_.size() - 1; ++i) {
        board_[i] = i + 1;
    }
    board_.back() = 0; // empty tile at the end
    empty_pos_ = board_.size() - 1;

    std::random_device rd;
    std::mt19937 g(rd());

    do {
        // Shuffle all tiles except the last one (empty tile)
        std::shuffle(board_.begin(), board_.end() - 1, g);
    } while (!isSolvable()); // Keep shuffling until we get a solvable puzzle
}

bool NPuzzle::isSolvable() const {
    size_t inversionCount = 0;
    size_t puzzleSize = board_.size();
    
    // Count inversions
    for (size_t i = 0; i < puzzleSize - 1; ++i) {
        if (board_[i] == 0) continue;
        for (size_t j = i + 1; j < puzzleSize; ++j) {
            if (board_[j] == 0) continue;
            if (board_[i] > board_[j]) {
                inversionCount++;
            }
        }
    }

    // For odd-sized puzzles, the inversion count must be even
    if (size_ % 2 == 1) {
        return inversionCount % 2 == 0;
    }
    // For even-sized puzzles
    else {
        size_t emptyRow = empty_pos_ / size_;
        // Inversion count + row of empty tile must be odd
        return (inversionCount + emptyRow) % 2 == 1;
    }
}

void NPuzzle::set_heuristic(std::shared_ptr<Heuristic> heuristic) {
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

std::vector<std::shared_ptr<NPuzzle>> NPuzzle::generate_moves() const {
    std::vector<std::shared_ptr<NPuzzle>> moves;
    const size_t row = empty_pos_ / size_;
    const size_t col = empty_pos_ % size_;

    auto make_shared_swapped = [this](size_t new_pos) {
        auto np = std::make_shared<NPuzzle>(this->create_swapped(new_pos));
        np->set_heuristic(this->heuristic_);
        return np;
    };

    if (row > 0) moves.push_back(make_shared_swapped(empty_pos_ - size_));
    if (row < size_ - 1) moves.push_back(make_shared_swapped(empty_pos_ + size_));
    if (col > 0) moves.push_back(make_shared_swapped(empty_pos_ - 1));
    if (col < size_ - 1) moves.push_back(make_shared_swapped(empty_pos_ + 1));

    return moves;
}

bool NPuzzle::operator==(const NPuzzle& other) const {
    return board_ == other.board_;
}

bool NPuzzle::operator!=(const NPuzzle& other) const { return !(*this == other); }

NPuzzle& NPuzzle::operator=(const NPuzzle& other) {
    if (this != &other) {
        size_ = other.size_;
        board_ = other.board_;
        empty_pos_ = other.empty_pos_;
        g = other.g;
        if (other.heuristic_) {
            heuristic_ = std::unique_ptr<Heuristic>(other.heuristic_->clone());
        } else {
            heuristic_.reset();
        }
    }
    return *this;
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

int NPuzzle::total_cost() const {
    if (heuristic_) {
        return g + (*heuristic_)(board_);
    }
    return g; // fallback if no heuristic
}