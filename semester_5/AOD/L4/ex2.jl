using ArgParse
using Random
using DataStructures
using Printf
using JuMP
import GLPK

# Implementacja algorytmu Hopcrofta-Karpa
function hopcroft_karp(adj, n1, n2)
    pair1 = fill(0, n1)
    pair2 = fill(0, n2)
    dist = fill(0, n1 + 1)
    matching_size = 0
    
    function bfs()
        queue = Deque{Int}()
        for u in 1:n1
            if pair1[u] == 0
                dist[u] = 0
                push!(queue, u)
            else
                dist[u] = typemax(Int)
            end
        end
        dist[n1 + 1] = typemax(Int)
        
        while !isempty(queue)
            u = popfirst!(queue)
            if dist[u] < dist[n1 + 1]
                for v in adj[u]
                    if dist[pair2[v] == 0 ? n1 + 1 : pair2[v]] == typemax(Int)
                        dist[pair2[v] == 0 ? n1 + 1 : pair2[v]] = dist[u] + 1
                        if pair2[v] != 0
                            push!(queue, pair2[v])
                        end
                    end
                end
            end
        end
        return dist[n1 + 1] != typemax(Int)
    end
    
    function dfs(u)
        if u != 0
            for v in adj[u]
                if dist[pair2[v] == 0 ? n1 + 1 : pair2[v]] == dist[u] + 1
                    if dfs(pair2[v])
                        pair2[v] = u
                        pair1[u] = v
                        return true
                    end
                end
            end
            dist[u] = typemax(Int)
            return false
        end
        return true
    end
    
    while bfs()
        for u in 1:n1
            if pair1[u] == 0 && dfs(u)
                matching_size += 1
            end
        end
    end
    
    return matching_size, pair1
end

function generate_bipartite_graph(k, i_deg)
    n = 2^k
    adj = [Int[] for _ in 1:n]
    for u in 1:n
        neighbors = rand(1:n, i_deg)
        adj[u] = unique(neighbors)
    end
    return adj
end

# --- NOWA FUNKCJA: Generowanie modelu LP dla skojarzenia  ---
function save_matching_lp(adj, n1, n2, filename)
    println("Generowanie modelu LP (Matching) do pliku: $filename ...")
    model = Model()

    # Zbieramy wszystkie możliwe krawędzie
    edges_list = []
    for u in 1:n1
        for v in adj[u]
            push!(edges_list, (u, v))
        end
    end

    # Zmienne binarne: x[u,v] (czy krawędź jest w skojarzeniu)
    @variable(model, x[1:length(edges_list)], Bin)

    # Ograniczenia dla wierzchołków z V1: suma krawędzi wychodzących <= 1
    for u in 1:n1
        u_indices = [i for (i, (from, to)) in enumerate(edges_list) if from == u]
        if !isempty(u_indices)
            set_name(sum(x[i] for i in u_indices), "V1_node_$u")
            @constraint(model, sum(x[i] for i in u_indices) <= 1)
        end
    end

    # Ograniczenia dla wierzchołków z V2: suma krawędzi wchodzących <= 1
    for v in 1:n2
        v_indices = [i for (i, (from, to)) in enumerate(edges_list) if to == v]
        if !isempty(v_indices)
            set_name(sum(x[i] for i in v_indices), "V2_node_$v")
            @constraint(model, sum(x[i] for i in v_indices) <= 1)
        end
    end

    # Funkcja celu: maksymalizacja liczby krawędzi
    @objective(model, Max, sum(x))

    write_to_file(model, filename)
    println("Zapisano model.")
end

function parse_commandline()
    s = ArgParseSettings()
    @add_arg_table! s begin
        "--size", "-k"
            arg_type = Int
            required = true
        "--degree", "-i"
            arg_type = Int
            required = true
        "--printMatching"
            action = :store_true
        "--glpk"
            arg_type = String
            required = false
            help = "Nazwa pliku wyjściowego dla modelu LP"
    end
    return parse_args(s)
end

function main()
    args = parse_commandline()
    k = args["size"]
    i_deg = args["degree"]
    size_v = 2^k
    
    adj = generate_bipartite_graph(k, i_deg)
    
    # Obsługa generowania pliku LP (Zadanie 3) [cite: 110]
    if args["glpk"] !== nothing
        save_matching_lp(adj, size_v, size_v, args["glpk"])
    end

    start_time = time()
    m_size, matches = hopcroft_karp(adj, size_v, size_v)
    elapsed = time() - start_time
    
    println("Maksymalne skojarzenie: $m_size")
    
    if args["printMatching"]
        println("Lista krawędzi skojarzenia (V1 -> V2):")
        for (u, v) in enumerate(matches)
            if v != 0
                println("$u -> $v")
            end
        end
    end
    
    @warn "Czas działania" Czas_s=elapsed
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end