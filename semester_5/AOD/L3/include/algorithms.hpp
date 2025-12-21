#pragma once
#include <vector>
#include <string>
#include <limits>
#include "graph.hpp"


void runDijkstraStandard(const Graph& g, int startNode, std::vector<long long>& dist);

void runDial(const Graph& g, int startNode, std::vector<long long>& dist);

void runRadixHeap(const Graph& g, int startNode, std::vector<long long>& dist);

