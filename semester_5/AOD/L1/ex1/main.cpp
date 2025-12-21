#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

class Graph {
private:
    int V;
    std::vector<std::list<int>> adj; 
    bool is_directed; 

    void DFSUtil(int v, std::vector<bool>& visited, std::vector<int>& parent) {
        visited[v] = true;
        std::cout << v << " ";

        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                parent[neighbor] = v;
                DFSUtil(neighbor, visited, parent);
            }
        }
    }

public:
    Graph(int V, bool directed = false) : V(V), is_directed(directed) {
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        if (!is_directed) {
            adj[v].push_back(u); 
        }
    }

     void export_tree_to_dot(const std::vector<int>& parent, const std::string& traversal_type, const std::string& filename_prefix) {
        std::string filename = filename_prefix + "_" + traversal_type + "_tree.dot";
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Could not open file for writing: " << filename << std::endl;
            return;
        }

        file << (is_directed ? "digraph G {" : "graph G {") << std::endl;
        file << "  label=\"" << traversal_type << " Tree for " << filename_prefix << "\";" << std::endl;

        // Draw all graph edges in grey
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                if (is_directed || u < v) {
                    file << "  " << u << (is_directed ? " -> " : " -- ") << v << " [color=gray];" << std::endl;
                }
            }
        }

        // Highlight tree edges in red
        for (int i = 0; i < V; ++i) {
            if (parent[i] != -1) {
                file << "  " << parent[i] << (is_directed ? " -> " : " -- ") << i << " [color=red, penwidth=2.0];" << std::endl;
            }
        }

        file << "}" << std::endl;
        std::cout << "Exported " << traversal_type << " tree to " << filename << std::endl;
    }

    void BFS(int start_node, bool print_tree, const std::string& filename_prefix = "graph") {
        std::cout << "BFS starting from vertex " << start_node << ": ";
        std::vector<bool> visited(V, false);
        std::vector<int> parent(V, -1);
        std::queue<int> q;

        visited[start_node] = true;
        q.push(start_node);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            std::cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        std::cout << std::endl;

        if (print_tree) {
            std::cout << "BFS Tree:" << std::endl;
            for (int i = 0; i < V; ++i) {
                if (parent[i] != -1) {
                    std::cout << "  " << parent[i] << " -> " << i << std::endl;
                }
            }
            export_tree_to_dot(parent, "BFS", filename_prefix);
        }
    }

    void DFS(int start_node, bool print_tree, const std::string& name_prefix = "graph") {
        std::cout << "DFS starting from vertex " << start_node << ": ";
        std::vector<bool> visited(V, false);
        std::vector<int> parent(V, -1);

        DFSUtil(start_node, visited, parent);

        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                DFSUtil(i, visited, parent);
            }
        }
        std::cout << std::endl;

        if (print_tree) {
            std::cout << "DFS Tree:" << std::endl;
            for (int i = 0; i < V; ++i) {
                if (parent[i] != -1) {
                    std::cout << "  " << parent[i] << " -> " << i << std::endl;
                }
            }

            export_tree_to_dot(parent, "DFS", name_prefix);
        }
    }
};

void run_test(const std::string& test_name, int num_vertices, const std::vector<std::pair<int, int>>& edges, bool print_tree) {
    std::cout << "--- " << test_name << " ---" << std::endl;

    // Directed
    std::string directed_name = test_name + "_directed";
    std::cout << "--- DIRECTED ---" << std::endl;
    Graph g_dir(num_vertices, true);
    for (const auto& edge : edges) {
        g_dir.addEdge(edge.first, edge.second);
    }

    auto start = std::chrono::high_resolution_clock::now();
    g_dir.BFS(0, print_tree, directed_name);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> bfs_dir_time = end - start;
    std::cout << "BFS (Directed) time: " << bfs_dir_time.count() << " ms" << std::endl;
    std::cout << std::endl;

    start = std::chrono::high_resolution_clock::now();
    g_dir.DFS(0, print_tree, directed_name);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dfs_dir_time = end - start;
    std::cout << "DFS (Directed) time: " << dfs_dir_time.count() << " ms" << std::endl;
    std::cout << std::endl;

    // Undirected
    std::string undirected_name = test_name + "_undirected";
    std::cout << "--- UNDIRECTED ---" << std::endl;
    Graph g_undir(num_vertices, false);
    for (const auto& edge : edges) {
        g_undir.addEdge(edge.first, edge.second);
    }

    start = std::chrono::high_resolution_clock::now();
    g_undir.BFS(0, print_tree, undirected_name);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> bfs_undir_time = end - start;
    std::cout << "BFS (Undirected) time: " << bfs_undir_time.count() << " ms" << std::endl;
    std::cout << std::endl;

    start = std::chrono::high_resolution_clock::now();
    g_undir.DFS(0, print_tree, undirected_name);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dfs_undir_time = end - start;
    std::cout << "DFS (Undirected) time: " << dfs_undir_time.count() << " ms" << std::endl;
    std::cout << "------------------------" << std::endl << std::endl;
}

void run_undirected_graph_tests(bool print_tree) {
    std::cout << "--- UNDIRECTED GRAPH ---" << std::endl;
    Graph g(10, false); // 10 wierzchołków, nieskierowany
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    g.addEdge(3, 7);
    g.addEdge(4, 8);
    g.addEdge(5, 9);

    g.BFS(0, print_tree);
    std::cout << std::endl;
    g.DFS(0, print_tree);
    std::cout << "------------------------" << std::endl << std::endl;
}

void run_directed_graph_tests(bool print_tree) {
    std::cout << "--- DIRECTED GRAPH ---" << std::endl;
    Graph g(10, true); // 10 wierzchołków, skierowany
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(3, 2); // Cykl
    g.addEdge(4, 6);
    g.addEdge(5, 7);
    g.addEdge(6, 5);
    g.addEdge(7, 8);
    g.addEdge(8, 9);
    g.addEdge(9, 6);

    g.BFS(0, print_tree);
    std::cout << std::endl;
    g.DFS(0, print_tree);
    std::cout << "----------------------" << std::endl << std::endl;
}

void run_graph1_tests(bool print_tree) {
    run_test("GRAPH_1", 6, {
        {0, 1}, {0, 2}, {1, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5}, {3, 4}, {4, 5},
    }, print_tree);
}

void run_graph2_tests(bool print_tree) {
    run_test("GRAPH_2", 8, {
        {0, 1}, {1, 2}, {2, 3}, {0, 3}, // Outer square
        {4, 7}, {7, 6}, {5, 6}, {5, 4}, // Inner square
        {4, 0}, {1, 5}, {6, 2}, {3, 7}  // Connections between squares
    }, print_tree);
}

void run_graph3_tests(bool print_tree) {
    run_test("GRAPH_3", 9, {
        {0, 1}, {0, 2}, {0, 4},
        {1, 3}, {1, 4},
        {2, 4}, {2, 5},
        {3, 7}, {3, 4},
        {4, 5},
        {5, 6}, {5, 8},
        {6, 3}, {6, 4}, {6, 7},
        {8, 6}, {8, 7}
    }, print_tree);
}


int main(int argc, char* argv[]) {
    bool print_tree = false;
    if (argc > 1 && std::string(argv[1]) == "tree") {
        print_tree = true;
    }

    run_graph1_tests(print_tree);
    run_graph2_tests(print_tree);
    run_graph3_tests(print_tree);

    return 0;
}