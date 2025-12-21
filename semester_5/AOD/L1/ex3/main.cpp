#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <stack>
#include <chrono>

#include "reader.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


std::pair<std::vector<std::vector<int>>, std::vector<int>> reverseGraph(const std::vector<std::vector<int>> &graph) {
    int n = graph.size();
    std::vector<std::vector<int>> reversedGraph(n);
    for (int u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            if (v >= 0 && v < n) { 
                reversedGraph[v].push_back(u);
            }
        }
    }

    std::vector<bool> visited(n, false);
    std::vector<int> finishedOrder;
    finishedOrder.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            std::stack<int> s;
            s.push(i);
            
            while(!s.empty()){
                int u = s.top();

                if (!visited[u]) {
                    visited[u] = true;
                    for(int v : graph[u]){
                        if(!visited[v]){
                            s.push(v);
                        }
                    }
                } else {
                    s.pop();
                    finishedOrder.push_back(u);
                }
            }
        }
    }

    std::reverse(finishedOrder.begin(), finishedOrder.end());

    return {reversedGraph, finishedOrder};
}

std::vector<std::vector<int>> stronglyConnectedComponents(const std::vector<std::vector<int>> &graph) {
    int n = graph.size();
    if (n == 0) {
        return {};
    }
    auto [rGraph, order] = reverseGraph(graph);

    std::vector<std::vector<int>> sccs;
    std::vector<bool> visited(n, false);
    std::stack<int> s;
    
    for(int i : order) {
        if(!visited[i]) {
            std::vector<int> current_scc;
            s.push(i);
            visited[i] = true;
            
            while(!s.empty()) {
                int u = s.top();
                s.pop();
                current_scc.push_back(u);

                for(int v : rGraph[u]) { 
                    if(!visited[v]) {
                        visited[v] = true;
                        s.push(v);
                    }
                }
            }
            sccs.push_back(current_scc);
        }
    }
    
    return sccs;
}



int main(int argc, char* argv[]) {
    const std::string test_dir = TOSTRING(TEST_DIR);
    std::vector<std::string> files_to_process = {
        test_dir + "/3/g3-1.txt",
        test_dir + "/3/g3-2.txt",
        test_dir + "/3/g3-3.txt",
        test_dir + "/3/g3-4.txt",
        test_dir + "/3/g3-5.txt",
        test_dir + "/3/g3-6.txt",
    };

    std::vector<GraphData> all_graphs_data;
    all_graphs_data.reserve(files_to_process.size());

    int file_counter = 1;
    for (const auto& file_path : files_to_process) {
        try {
            std::cout << "Processing file: " << file_path << std::endl;
            GraphData graph_data = read_graph_from_file(file_path);

            auto start = std::chrono::high_resolution_clock::now();
            auto sccs = stronglyConnectedComponents(graph_data.adj_list);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> diff = end - start;

            std::cout << "Time taken: " << diff.count() << " ms" << std::endl;

            std::cout << "Liczba silnie spójnych składowych: " << sccs.size() << std::endl;

            for (auto& component : sccs) {
                std::cout << "  Liczba wierzchołków w składowej: " << component.size() << std::endl;
                if (graph_data.num_vertices <= 200) {
                    //std::sort(component.begin(), component.end());
                    std::cout << "    Wierzchołki: ";
                    for (int vertex : component) {
                        // Wypisywanie wierzchołków z indeksowaniem od 1
                        std::cout << vertex + 1 << " ";
                    }
                    std::cout << std::endl;
                }
            }

            std::string output_filename = "scc_graph_" + std::to_string(file_counter++) + ".dot";
            export_scc_to_dot(graph_data, sccs, output_filename);
            std::cout << "Exported SCC visualization to " << output_filename << std::endl;
            
        } catch (const std::runtime_error& e) {
            std::cerr << "Error processing file " << file_path << ": " << e.what() << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    return 0;
}