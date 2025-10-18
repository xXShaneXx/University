#ifndef READER_HPP
#define READER_HPP

#include <iostream>
#include <vector>
#include <string>

struct GraphData {
    char type;
    int num_vertices;
    int num_edges;
    std::vector<std::vector<int>> adj_list;
};

GraphData read_graph_from_stream(std::istream& is);
GraphData read_graph_from_file(const std::string& filename);
void export_to_dot(const std::vector<GraphData>& graphs, const std::string& filename);
void export_scc_to_dot(const GraphData& graph_data, const std::vector<std::vector<int>>& sccs, const std::string& filename);
void export_topological_sort_to_dot(const GraphData& graph_data, const std::vector<int>& sorted_nodes, const std::string& filename);
void export_bipartite_to_dot(const GraphData& graph_data, const std::vector<int>& set_red, const std::vector<int>& set_black, const std::string& filename);

#endif // READER_HPP
