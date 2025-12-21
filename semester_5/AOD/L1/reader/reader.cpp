#include "reader.hpp"
#include <fstream>

GraphData read_graph_from_stream(std::istream& is) {
    GraphData data;
    is >> data.type;
    is >> data.num_vertices;
    is >> data.num_edges;

    data.adj_list.resize(data.num_vertices);

    for (int i = 0; i < data.num_edges; ++i) {
        int u, v;
        is >> u >> v;
        // Adjust to 0-based indexing
        u--; 
        v--;
        data.adj_list[u].push_back(v);
        if (data.type == 'U') {
            data.adj_list[v].push_back(u);
        }
    }

    return data;
}

GraphData read_graph_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    return read_graph_from_stream(file);
}

void export_to_dot(const std::vector<GraphData>& graphs, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    // Main graph container
    file << "digraph AllGraphs {" << std::endl;
    file << "  compound=true;" << std::endl;

    int graph_counter = 0;
    for (const auto& graph_data : graphs) {
        file << "  subgraph cluster_" << graph_counter++ << " {" << std::endl;
        file << "    label = \"Graph " << graph_counter << "\";" << std::endl;

        const char edge_op = (graph_data.type == 'D') ? '>' : '-';

        for (int u = 0; u < graph_data.num_vertices; ++u) {
            for (int v : graph_data.adj_list[u]) {
                // For undirected, only draw edge once
                if (graph_data.type == 'U' && u > v) {
                    continue;
                }
                // Use 1-based indexing for visualization
                file << "    " << u + 1 << " -" << edge_op << " " << v + 1 << ";" << std::endl;
            }
        }
        file << "  }" << std::endl;
    }

    file << "}" << std::endl;
}

void export_scc_to_dot(const GraphData& graph_data, const std::vector<std::vector<int>>& sccs, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "digraph G {" << std::endl;
    file << "  compound=true;" << std::endl;
    file << "  label=\"" << filename << "\";" << std::endl;

    // Define all edges first
    const char edge_op = (graph_data.type == 'D') ? '>' : '-';
    for (int u = 0; u < graph_data.num_vertices; ++u) {
        for (int v : graph_data.adj_list[u]) {
            if (graph_data.type == 'U' && u > v) continue;
            file << "  " << u + 1 << " -" << edge_op << " " << v + 1 << ";" << std::endl;
        }
    }

    // Define clusters for each SCC
    int scc_counter = 0;
    // A list of colors for different SCCs
    const std::vector<std::string> colors = {"coral", "lightblue", "lightgreen", "khaki", "orchid", "lightgrey", "pink"};

    for (const auto& component : sccs) {
        file << "  subgraph cluster_" << scc_counter << " {" << std::endl;
        file << "    label = \"SCC " << scc_counter + 1 << "\";" << std::endl;
        file << "    style=filled;" << std::endl;
        file << "    color=\"" << colors[scc_counter % colors.size()] << "\";" << std::endl;
        
        for (int vertex : component) {
            file << "    " << vertex + 1 << ";" << std::endl;
        }
        file << "  }" << std::endl;
        scc_counter++;
    }

    file << "}" << std::endl;
}

void export_topological_sort_to_dot(const GraphData& graph_data, const std::vector<int>& sorted_nodes, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "digraph G {" << std::endl;
    file << "  rankdir=LR;" << std::endl;
    file << "  label=\"" << filename << "\";" << std::endl;

    // Define all edges first
    for (int u = 0; u < graph_data.num_vertices; ++u) {
        for (int v : graph_data.adj_list[u]) {
            file << "  " << u + 1 << " -> " << v + 1 << ";" << std::endl;
        }
    }

    // Add invisible edges to enforce topological order visually
    file << "  { rank = same; ";
    for (size_t i = 0; i < sorted_nodes.size() - 1; ++i) {
        file << sorted_nodes[i] + 1 << " -> " << sorted_nodes[i+1] + 1 << " [style=invis];" << std::endl;
    }
    file << "  }" << std::endl;

    file << "}" << std::endl;
}

void export_bipartite_to_dot(const GraphData& graph_data, const std::vector<int>& set_red, const std::vector<int>& set_black, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "graph G {" << std::endl;
    file << "  layout=neato;" << std::endl;
    file << "  label=\"" << filename << "\";" << std::endl;

    // Define nodes for the red set
    file << "  subgraph cluster_0 {" << std::endl;
    file << "    label = \"Set V0 (Red)\";" << std::endl;
    file << "    color=red;" << std::endl;
    file << "    node [style=filled, color=coral];" << std::endl;
    for (int node : set_red) {
        file << "    " << node << ";" << std::endl;
    }
    file << "  }" << std::endl;

    // Define nodes for the black set
    file << "  subgraph cluster_1 {" << std::endl;
    file << "    label = \"Set V1 (Black)\";" << std::endl;
    file << "    color=black;" << std::endl;
    file << "    node [style=filled, color=lightblue];" << std::endl;
    for (int node : set_black) {
        file << "    " << node << ";" << std::endl;
    }
    file << "  }" << std::endl;

    // Define all edges
    for (int u = 0; u < graph_data.num_vertices; ++u) {
        for (int v : graph_data.adj_list[u]) {
            if (graph_data.type == 'U' && u > v) continue;
            file << "  " << u + 1 << " -- " << v + 1 << ";" << std::endl;
        }
    }

    file << "}" << std::endl;
}

