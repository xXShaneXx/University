#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>
#include "reader.hpp"
#include <string>
#include <fstream>
#include <optional>
#include <chrono>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

using namespace std;

bool dfsVisit(int node, const vector<vector<int>> &graph, vector<bool> &visited, vector<bool>& recursionStack, stack<int>& st) {
    visited[node] = true;
    recursionStack[node] = true;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            if(dfsVisit(neighbor, graph, visited, recursionStack, st))
                return true;
        } else if (recursionStack[neighbor]) {
            return true;
        }
    }
    st.push(node);
    recursionStack[node] = false;
    return false;
}

optional<vector<int>> topSort(const vector<vector<int>> &graph) {
    int n = graph.size();
    stack<int> st;
    vector<bool> visited(n, false);
    vector<bool> recursionStack(n, false);
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            if(dfsVisit(i, graph, visited, recursionStack, st)){
                return {};
            }
        }
    }
    
    vector<int> result;
    while(!st.empty()){
        result.push_back(st.top());
        st.pop();
    }

    return result;
}

int main(int argc, char* argv[]) {
    const std::string test_dir = TOSTRING(TEST_DIR);
    std::vector<std::string> files_to_process = {
        test_dir + "/2/g2a-1.txt",
        test_dir + "/2/g2a-2.txt",
        test_dir + "/2/g2a-3.txt",
        test_dir + "/2/g2a-4.txt",
        test_dir + "/2/g2a-5.txt",
        test_dir + "/2/g2a-6.txt",
        
        test_dir + "/2/g2b-1.txt",
        test_dir + "/2/g2b-2.txt",
        test_dir + "/2/g2b-3.txt",
        test_dir + "/2/g2b-4.txt",
        test_dir + "/2/g2b-5.txt",
        test_dir + "/2/g2b-6.txt"
    };

    for (const auto& file_path : files_to_process) {
        try {
            std::cout << "Processing file: " << file_path << std::endl;
            GraphData graph_data = read_graph_from_file(file_path);

            if (graph_data.type == 'D') {
                auto start = std::chrono::high_resolution_clock::now();
                auto sorted_nodes_opt = topSort(graph_data.adj_list);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << "Time taken by topSort: " << duration.count() << " microseconds" << std::endl;

                if (!sorted_nodes_opt) {
                    std::cout << "Graph contains a directed cycle." << std::endl;
                } else {
                    std::cout << "Graph is acyclic." << std::endl;
                    if (graph_data.num_vertices <= 200) {
                        std::cout << "Topological sort: ";
                        for (int node : *sorted_nodes_opt) {
                            std::cout << node + 1 << " ";
                        }
                        std::cout << std::endl;

                        std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);
                        std::string dot_filename = "topological_sort_" + base_filename.substr(0, base_filename.find_last_of(".")) + ".dot";
                        export_topological_sort_to_dot(graph_data, *sorted_nodes_opt, dot_filename);
                        std::cout << "Exported topological sort to " << dot_filename << std::endl;
                    }
                }
            } else {
                std::cout << "Graph is not a directed graph. Topological sort is not applicable." << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Error processing file " << file_path << ": " << e.what() << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    return 0;
}