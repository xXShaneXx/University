# Zadanie 1
| Graph   | Type       | BFS order                  | BFS tree edges                                                                 | Export file                             | Time (ms) |
|---------|------------|----------------------------|---------------------------------------------------------------------------------|-----------------------------------------|-----------|
| GRAPH_1 | Directed   | 0 1 2 3 4 5                | 0 -> 1, 0 -> 2, 1 -> 3, 1 -> 4, 2 -> 5                                          | GRAPH_1_directed_BFS_tree.dot           | 0.390409  |
| GRAPH_1 | Undirected | 0 1 2 3 4 5                | 0 -> 1, 0 -> 2, 1 -> 3, 1 -> 4, 2 -> 5                                          | GRAPH_1_undirected_BFS_tree.dot         | 0.279373  |
| GRAPH_2 | Directed   | 0 1 3 2 5 7 6 4            | 0 -> 1, 1 -> 2, 0 -> 3, 5 -> 4, 1 -> 5, 5 -> 6, 3 -> 7                          | GRAPH_2_directed_BFS_tree.dot           | 0.258452  |
| GRAPH_2 | Undirected | 0 1 3 4 2 5 7 6            | 0 -> 1, 1 -> 2, 0 -> 3, 0 -> 4, 1 -> 5, 2 -> 6, 3 -> 7                          | GRAPH_2_undirected_BFS_tree.dot         | 0.280084  |
| GRAPH_3 | Directed   | 0 1 2 4 3 5 7 6 8          | 0 -> 1, 0 -> 2, 1 -> 3, 0 -> 4, 2 -> 5, 5 -> 6, 3 -> 7, 5 -> 8                  | GRAPH_3_directed_BFS_tree.dot           | 0.291297  |
| GRAPH_3 | Undirected | 0 1 2 4 3 5 6 7 8          | 0 -> 1, 0 -> 2, 1 -> 3, 0 -> 4, 2 -> 5, 4 -> 6, 3 -> 7, 5 -> 8                  | GRAPH_3_undirected_BFS_tree.dot         | 0.358026  |

| Graph   | Type       | DFS order                  | DFS tree edges                                                                 | Export file                             | Time (ms) |
|---------|------------|----------------------------|---------------------------------------------------------------------------------|-----------------------------------------|-----------|
| GRAPH_1 | Directed   | 0 1 2 4 5 3                | 0 -> 1, 1 -> 2, 1 -> 3, 2 -> 4, 4 -> 5                                          | GRAPH_1_directed_DFS_tree.dot           | 0.256559  |
| GRAPH_1 | Undirected | 0 1 2 4 3 5                | 0 -> 1, 1 -> 2, 4 -> 3, 2 -> 4, 4 -> 5                                          | GRAPH_1_undirected_DFS_tree.dot         | 0.227783  |
| GRAPH_2 | Directed   | 0 1 2 3 7 6 5 4            | 0 -> 1, 1 -> 2, 2 -> 3, 5 -> 4, 1 -> 5, 7 -> 6, 3 -> 7                          | GRAPH_2_directed_DFS_tree.dot           | 0.252481  |
| GRAPH_2 | Undirected | 0 1 2 3 7 4 5 6            | 0 -> 1, 1 -> 2, 2 -> 3, 7 -> 4, 4 -> 5, 5 -> 6, 3 -> 7                          | GRAPH_2_undirected_DFS_tree.dot         | 0.26206   |
| GRAPH_3 | Directed   | 0 1 3 7 4 5 6 8 2          | 0 -> 1, 0 -> 2, 1 -> 3, 3 -> 4, 4 -> 5, 5 -> 6, 3 -> 7, 5 -> 8                  | GRAPH_3_directed_DFS_tree.dot           | 0.285976  |
| GRAPH_3 | Undirected | 0 1 3 7 6 5 2 4 8          | 0 -> 1, 5 -> 2, 1 -> 3, 2 -> 4, 6 -> 5, 7 -> 6, 3 -> 7, 5 -> 8                  | GRAPH_3_undirected_DFS_tree.dot         | 0.286327  |

# Zadanie 2

| File     | Time (μs) | Result                      | Topological Sort (first 20 nodes)                                                |
|----------|-----------|-----------------------------|----------------------------------------------------------------------------------| 
| g2a-1    | 24        | Graph is acyclic            | 1 5 9 13 2 6 10 14 3 7 11 15 4 8 12 16                                          |
| g2a-2    | 136       | Graph is acyclic            | 1 11 21 31 41 51 61 71 81 91 2 12 22 32 42 52 62 72 82 92...                    |
| g2a-3    | 1776      | Graph is acyclic            | -                                                                                |
| g2a-4    | 6361      | Graph is acyclic            | -                                                                                |
| g2a-5    | 158101    | Graph is acyclic            | -                                                                                |
| g2a-6    | 1123046   | Graph is acyclic            | -                                                                                |
| g2b-1    | 7         | Graph contains a cycle      | -                                                                                |
| g2b-2    | 25        | Graph contains a cycle      | -                                                                                |
| g2b-3    | 403       | Graph contains a cycle      | -                                                                                |
| g2b-4    | 2628      | Graph contains a cycle      | -                                                                                |
| g2b-5    | 63788     | Graph contains a cycle      | -                                                                                |
| g2b-6    | 466062    | Graph contains a cycle      | -                                                                                |

# Zadanie 3

| File  | Time (ms) | SCC Count | Component Sizes            | Export file        |
|-------|-----------|-----------|----------------------------|--------------------|
| g3-1  | 0.022183  | 5         | 5, 4, 4, 2, 1              | scc_graph_1.dot    |
| g3-2  | 0.088883  | 5         | 6, 40, 36, 24, 1           | scc_graph_2.dot    |
| g3-3  | 0.739524  | 5         | 7, 400, 400, 200, 1        | scc_graph_3.dot    |
| g3-4  | 7.75503   | 5         | 8, 4000, 3600, 2400, 1     | scc_graph_4.dot    |
| g3-5  | 73.5834   | 5         | 9, 40000, 40000, 20000, 1  | scc_graph_5.dot    |
| g3-6  | 719.06    | 5         | 10, 400000, 360000, 240000, 1 | scc_graph_6.dot |

# Zadanie 4
| File   | Time (ms) | Result                 | V0 (Red)                                                                                                                     | V1 (Black)                                                                                                                  | Export file                 |
|--------|-----------|------------------------|-------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------|-----------------------------|
| d4a-1  | 0.002765  | Graph is bipartite     | 1 3 6 8 9 11 14 16                                                                                                           | 2 4 5 7 10 12 13 15                                                                                                        | bipartite_d4a-1.dot        |
| d4a-2  | 0.01051   | Graph is bipartite     | 1 3 5 7 9 12 14 16 18 20 21 23 25 27 29 32 34 36 38 40 41 43 45 47 49 52 54 56 58 60 61 63 65 67 69 72 74 76 78 80 81 83 85 87 89 92 94 96 98 100 | 2 4 6 8 10 11 13 15 17 19 22 24 26 28 30 31 33 35 37 39 42 44 46 48 50 51 53 55 57 59 62 64 66 68 70 71 73 75 77 79 82 84 86 88 90 91 93 95 97 99 | bipartite_d4a-2.dot        |
| d4a-3  | 0.145663  | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| d4a-4  | 0.94334   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| d4a-5  | 15.6233   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| d4a-6  | 111.484   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-1  | 0.001894  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-2  | 0.005221  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-3  | 0.076068  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-4  | 0.537763  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-5  | 8.10009   | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| d4b-6  | 54.7518   | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4a-1  | 0.003647  | Graph is bipartite     | 1 4 5 6 7                                                                                                                     | 2 3 8 9 10 11 12 13 14 15                                                                                                   | bipartite_u4a-1.dot        |
| u4a-2  | 0.010801  | Graph is bipartite     | 1 4 5 6 7 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 | 2 3 8 9 10 11 12 13 14 15 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 | bipartite_u4a-2.dot        |
| u4a-3  | 0.091538  | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| u4a-4  | 4.42323   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| u4a-5  | 10.6757   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| u4a-6  | 88.4953   | Graph is bipartite     | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-1  | 0.002014  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-2  | 0.004619  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-3  | 0.034587  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-4  | 0.542713  | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-5  | 4.39943   | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |
| u4b-6  | 36.0861   | Graph is not bipartite | -                                                                                                                             | -                                                                                                                           | -                           |

