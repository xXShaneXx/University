#include "board.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <ctime>
#include <limits>

// --- Parameters ---
constexpr int POP_SIZE = 30;
constexpr int GENERATIONS = 40;
constexpr int WEIGHTS_COUNT = 5; // [my_3, my_2, my_1, opp_3, center]
constexpr double MUTATION_RATE = 0.2;
constexpr int TOURNAMENT_SIZE = 3;

std::mt19937 rng(std::random_device{}());

// --- Individual ---
struct Individual {
    std::vector<double> weights;
    double fitness = 0.0;
};

// --- Feature extraction (matches your heuristic) ---
std::vector<int> extractFeatures(const Board& board, int player) {
    auto& b = board.getBoard();
    int my_3 = 0, my_2 = 0, my_1 = 0, opp_3 = 0, center = 0;

    // Winning lines
    for (const auto& line : Board::winningLines) {
        int pc = 0, oc = 0;
        for (const auto& [i, j] : line) {
            if (b[i][j] == player) pc++;
            else if (b[i][j] == 3 - player) oc++;
        }
        if (pc > 0 && oc == 0) {
            if (pc == 3) my_3++;
            else if (pc == 2) my_2++;
            else if (pc == 1) my_1++;
        }
        if (oc > 0 && pc == 0) {
            if (oc == 3) opp_3++;
        }
    }

    // Center control
    if (Board::SIZE % 2 != 0) {
        int c = Board::SIZE / 2;
        if (b[c][c] == player) center = 1;
        else if (b[c][c] == 3 - player) center = -1;
    }
    return {my_3, my_2, my_1, opp_3, center};
}

// --- Heuristic using weights ---
double evaluateHeuristicWithWeights(const Board& board, int player, const std::vector<double>& weights) {
    auto features = extractFeatures(board, player);
    double score = 0.0;
    for (size_t i = 0; i < features.size(); ++i)
        score += features[i] * weights[i];
    return score;
}

// --- Random move for random bot ---
int randomMove(Board& board) {
    auto& b = board.getBoard();
    std::vector<int> moves;
    for (int i = 0; i < Board::SIZE; ++i)
        for (int j = 0; j < Board::SIZE; ++j)
            if (b[i][j] == 0)
                moves.push_back((i + 1) * 10 + (j + 1));
    if (moves.empty()) return -1;
    return moves[std::uniform_int_distribution<>(0, moves.size() - 1)(rng)];
}

// --- Play a single game with the given weights, return 1=win, 0=draw, -1=loss for the weighted bot ---
int playGameWithWeights(const std::vector<double>& weights, int firstPlayer) {
    Board board;
    int currentPlayer = firstPlayer;
    int movesLeft = Board::SIZE * Board::SIZE;
    while (true) {
        int move = -1;
        if (currentPlayer == 1) {
            // Weighted bot: pick move with best heuristic
            int bestScore = -1000000, bestMove = -1;
            auto& b = board.getBoard();
            for (int i = 0; i < Board::SIZE; ++i) {
                for (int j = 0; j < Board::SIZE; ++j) {
                    if (b[i][j] == 0) {
                        b[i][j] = 1;
                        double score = evaluateHeuristicWithWeights(board, 1, weights);
                        b[i][j] = 0;
                        if (score > bestScore) {
                            bestScore = score;
                            bestMove = (i + 1) * 10 + (j + 1);
                        }
                    }
                }
            }
            move = bestMove;
        } else {
            // Random bot
            move = randomMove(board);
        }
        if (move == -1) break;
        board.setMove(move, currentPlayer);
        movesLeft--;
        // Check win/lose
        if (board.winCheck(currentPlayer)) return (currentPlayer == 1) ? 1 : -1;
        if (movesLeft == 0) return 0;
        currentPlayer = 3 - currentPlayer;
    }
    return 0;
}

// --- Fitness: average result over N games (as both first and second player) ---
double fitnessFunction(const std::vector<double>& weights) {
    int N = 4;
    int total = 0;
    for (int i = 0; i < N; ++i) {
        total += playGameWithWeights(weights, 1); // bot starts
        total += playGameWithWeights(weights, 2); // bot second
    }
    return static_cast<double>(total) / (2 * N); // Range: [-1, 1]
}

// --- GA operators ---
Individual randomIndividual() {
    Individual ind;
    ind.weights.resize(WEIGHTS_COUNT);
    for (auto& w : ind.weights)
        w = std::uniform_real_distribution<>(-2000, 2000)(rng);
    return ind;
}

Individual crossover(const Individual& a, const Individual& b) {
    Individual child;
    child.weights.resize(WEIGHTS_COUNT);
    for (int i = 0; i < WEIGHTS_COUNT; ++i)
        child.weights[i] = (rng() % 2) ? a.weights[i] : b.weights[i];
    return child;
}

void mutate(Individual& ind) {
    for (auto& w : ind.weights)
        if (std::uniform_real_distribution<>(0, 1)(rng) < MUTATION_RATE)
            w += std::normal_distribution<>(0, 100)(rng);
}

Individual tournamentSelect(const std::vector<Individual>& pop) {
    Individual best = pop[rng() % pop.size()];
    for (int i = 1; i < TOURNAMENT_SIZE; ++i) {
        const Individual& challenger = pop[rng() % pop.size()];
        if (challenger.fitness > best.fitness)
            best = challenger;
    }
    return best;
}

// --- Main GA loop ---
int main() {
    std::vector<Individual> population(POP_SIZE);
    for (auto& ind : population)
        ind = randomIndividual();

    for (int gen = 0; gen < GENERATIONS; ++gen) {
        // Evaluate
        for (auto& ind : population)
            ind.fitness = fitnessFunction(ind.weights);

        // Sort by fitness
        std::sort(population.begin(), population.end(),
                  [](const Individual& a, const Individual& b) { return a.fitness > b.fitness; });

        std::cout << "Gen " << gen << " best fitness: " << population[0].fitness << "\n";

        // Elitism: keep top 2
        std::vector<Individual> newPop = {population[0], population[1]};

        // Generate rest
        while (newPop.size() < POP_SIZE) {
            Individual parent1 = tournamentSelect(population);
            Individual parent2 = tournamentSelect(population);
            Individual child = crossover(parent1, parent2);
            mutate(child);
            newPop.push_back(child);
        }
        population = std::move(newPop);
    }

    // Output best weights
    std::cout << "Best weights: ";
    for (auto w : population[0].weights)
        std::cout << w << " ";
    std::cout << std::endl;
    return 0;
}