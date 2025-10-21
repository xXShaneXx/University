#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include "reader.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

enum class Color {
    NO_COLOR,
    Red,
    Black
};

class Graph; 

bool isBipartite(Graph& graph);

class Graph {
private:
    int V; 
    std::vector<std::vector<int>> adj; 
    std::vector<Color> colors;

public:
    Graph(int V) : V(V), adj(V), colors(V, Color::NO_COLOR) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int getV() const { return V; }
    const std::vector<std::vector<int>>& getAdj() const { return adj; }

    void setColor(int vertex, Color color) {
        if (vertex >= 0 && vertex < V) {
            colors[vertex] = color;
        }
    }

    Color getColor(int vertex) const {
        if (vertex >= 0 && vertex < V) {
            return colors[vertex];
        }
        return Color::NO_COLOR;
    }

    const std::vector<Color>& getColors() const {
        return colors;
    }

    void resetColors() {
        colors.assign(V, Color::NO_COLOR);
    }
};

bool isBipartite(Graph& graph) {
    int V = graph.getV();
    const auto& adj = graph.getAdj();
    graph.resetColors(); 
    std::queue<int> q;

    for (int i = 0; i < V; ++i) {
        if (graph.getColor(i) == Color::NO_COLOR) {
            graph.setColor(i, Color::Red);
            q.push(i);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : adj[u]) {
                    if (graph.getColor(v) == Color::NO_COLOR) {
                        // color neighbor with opposite color
                        Color new_color = (graph.getColor(u) == Color::Red) ? Color::Black : Color::Red;
                        graph.setColor(v, new_color);
                        q.push(v);
                    } else if (graph.getColor(v) == graph.getColor(u)) {
                        // If neighbor has the same color, it is not bipartite
                        return false;
                    }
                }
            }
        }
    }
    return true;
}



int main(int argc, char* argv[]) {
    const std::string test_dir = TOSTRING(TEST_DIR);
    std::vector<std::string> files_to_process = {
        test_dir + "/4/d4a-1.txt",
        test_dir + "/4/d4a-2.txt",
        test_dir + "/4/d4a-3.txt",
        test_dir + "/4/d4a-4.txt",
        test_dir + "/4/d4a-5.txt",
        test_dir + "/4/d4a-6.txt",
        
        test_dir + "/4/d4b-1.txt",
        test_dir + "/4/d4b-2.txt",
        test_dir + "/4/d4b-3.txt",
        test_dir + "/4/d4b-4.txt",
        test_dir + "/4/d4b-5.txt",
        test_dir + "/4/d4b-6.txt",


        test_dir + "/4/u4a-1.txt",
        test_dir + "/4/u4a-2.txt",
        test_dir + "/4/u4a-3.txt",
        test_dir + "/4/u4a-4.txt",
        test_dir + "/4/u4a-5.txt",
        test_dir + "/4/u4a-6.txt",
        
        test_dir + "/4/u4b-1.txt",
        test_dir + "/4/u4b-2.txt",
        test_dir + "/4/u4b-3.txt",
        test_dir + "/4/u4b-4.txt",
        test_dir + "/4/u4b-5.txt",
        test_dir + "/4/u4b-6.txt",
    };

    for (const auto& file_path : files_to_process) {
        try {
            std::cout << "Processing file: " << file_path << std::endl;
            GraphData graph_data = read_graph_from_file(file_path);

            Graph graph(graph_data.num_vertices);
            for (int u = 0; u < graph_data.num_vertices; ++u) {
                for (int v : graph_data.adj_list[u]) {
                    if (u < v) {
                        graph.addEdge(u, v);
                    }
                }
            }
            if (graph_data.type == 'D') {
                 Graph directed_graph(graph_data.num_vertices);
                 for (int u = 0; u < graph_data.num_vertices; ++u) {
                    for (int v : graph_data.adj_list[u]) {
                        directed_graph.addEdge(u, v);
                    }
                }
                graph = directed_graph;
            }


            auto start = std::chrono::high_resolution_clock::now();
            bool is_bipartite = isBipartite(graph);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> diff = end - start;

            std::cout << "Time taken: " << diff.count() << " ms" << std::endl;

            if (is_bipartite) {
                std::cout << "Graph is bipartite." << std::endl;
                if (graph.getV() <= 200) {
                    std::vector<int> set_red, set_black;
                    const auto& colors = graph.getColors();
                    for (int i = 0; i < graph.getV(); ++i) {
                        if (colors[i] == Color::Red) {
                            set_red.push_back(i + 1);
                        } else if (colors[i] == Color::Black) {
                            set_black.push_back(i + 1);
                        }
                    }

                    std::cout << "Set V0 (Red): ";
                    for (int node : set_red) {
                        std::cout << node << " ";
                    }
                    std::cout << std::endl;

                    std::cout << "Set V1 (Black): ";
                    for (int node : set_black) {
                        std::cout << node << " ";
                    }
                    std::cout << std::endl;

                    std::string base_filename = file_path.substr(file_path.find_last_of("/\\") + 1);
                    std::string dot_filename = "bipartite_" + base_filename.substr(0, base_filename.find_last_of('.')) + ".dot";
                    export_bipartite_to_dot(graph_data, set_red, set_black, dot_filename);
                    std::cout << "Exported bipartite graph to " << dot_filename << std::endl;
                }
            } else {
                std::cout << "Graph is not bipartite." << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Error processing file " << file_path << ": " << e.what() << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    return 0;
}