using DataStructures
using Random
using Printf
using ArgParse
using JuMP
import GLPK # Importujemy dynamicznie do generowania modelu

# Struktura krawędzi dla sieci residualnej
mutable struct Edge
    to::Int
    capacity::Int
    flow::Int
    rev::Int # Indeks krawędzi powrotnej w liście sąsiedztwa celu
end

function build_hypercube(k::Int)
    n_nodes = 2^k
    adj = [Edge[] for _ in 1:n_nodes]
    
    for i in 0:n_nodes-1
        for bit in 0:k-1
            if (i >> bit) & 1 == 0
                j = i | (1 << bit)
                
                h_i = count_ones(i)
                z_i = k - h_i
                h_j = count_ones(j)
                z_j = k - h_j
                l = max(h_i, z_i, h_j, z_j)
                
                capacity = rand(1:2^l)
                
                push!(adj[i+1], Edge(j + 1, capacity, 0, length(adj[j+1]) + 1))
                push!(adj[j+1], Edge(i + 1, 0, 0, length(adj[i+1])))
            end
        end
    end
    return adj
end

function edmonds_karp(adj, s, t)
    n = length(adj)
    max_flow = 0
    path_count = 0
    
    while true
        parent = fill(0, n)
        edge_idx = fill(0, n)
        queue = Deque{Int}()
        
        push!(queue, s)
        parent[s] = -1
        
        found_path = false
        while !isempty(queue)
            u = popfirst!(queue)
            if u == t
                found_path = true
                break
            end
            
            for (idx, e) in enumerate(adj[u])
                if parent[e.to] == 0 && e.capacity - e.flow > 0
                    parent[e.to] = u
                    edge_idx[e.to] = idx
                    push!(queue, e.to)
                end
            end
            if found_path break end
        end
        
        if !found_path break end
        
        path_flow = typemax(Int)
        curr = t
        while curr != s
            p = parent[curr]
            idx = edge_idx[curr]
            path_flow = min(path_flow, adj[p][idx].capacity - adj[p][idx].flow)
            curr = p
        end
        
        curr = t
        while curr != s
            p = parent[curr]
            idx = edge_idx[curr]
            rev_idx = adj[p][idx].rev
            
            adj[p][idx].flow += path_flow
            adj[curr][rev_idx].flow -= path_flow
            curr = p
        end
        
        max_flow += path_flow
        path_count += 1
    end
    
    return max_flow, path_count
end

# --- NOWA FUNKCJA: Generowanie modelu LP  ---
function save_flow_lp(adj, s, t, filename)
    println("Generowanie modelu LP do pliku: $filename ...")
    model = Model() # Model bez solvera, tylko do zapisu

    n = length(adj)
    
    # Zbieramy wszystkie krawędzie (tylko te o dodatniej pojemności, nie residualne 0)
    edges_list = []
    for u in 1:n
        for e in adj[u]
            if e.capacity > 0
                push!(edges_list, (u, e.to, e.capacity))
            end
        end
    end

    # Zmienne: f[u,v] >= 0
    # Używamy nazw zmiennych jako "komentarzy" - f_u_v
    @variable(model, f[i=1:length(edges_list)] >= 0)
    
    # Ograniczenie przepustowości: f(u,v) <= c(u,v)
    for (i, (u, v, cap)) in enumerate(edges_list)
        set_name(f[i], "f_$(u-1)_$(v-1)") # Indeksy 0-based jak w treści zadania
        @constraint(model, f[i] <= cap)
    end

    # Bilans przepływu (Kirchhoffa) dla każdego węzła oprócz s i t
    for u in 1:n
        if u == s || u == t continue end
        
        # Wypływające z u
        out_indices = [i for (i, (from, to, _)) in enumerate(edges_list) if from == u]
        # Wpływające do u
        in_indices = [i for (i, (from, to, _)) in enumerate(edges_list) if to == u]
        
        if !isempty(out_indices) || !isempty(in_indices)
            @constraint(model, sum(f[i] for i in out_indices) == sum(f[i] for i in in_indices))
        end
    end

    # Funkcja celu: Max wypływ ze źródła
    source_out_indices = [i for (i, (from, to, _)) in enumerate(edges_list) if from == s]
    @objective(model, Max, sum(f[i] for i in source_out_indices))

    # Zapis do pliku w formacie CPLEX LP (czytelny dla GLPK)
    write_to_file(model, filename)
    println("Zapisano model.")
end

function parse_commandline()
    s = ArgParseSettings()
    @add_arg_table! s begin
        "--size", "-k"
            arg_type = Int
            required = true
            help = "Wymiar hiperkostki k"
        "--printFlow"
            action = :store_true
            help = "Wypisz przepływ na każdej krawędzi"
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
    s_node = 1
    t_node = 2^k
    
    adj = build_hypercube(k)
    
    # Obsługa generowania pliku LP (Zadanie 3) [cite: 110]
    if args["glpk"] !== nothing
        save_flow_lp(adj, s_node, t_node, args["glpk"])
    end

    start_time = time()
    max_f, p_count = edmonds_karp(adj, s_node, t_node)
    end_time = time()
    
    println("Maksymalny przepływ: $max_f")
    
    if args["printFlow"]
        println("\nPrzepływy na krawędziach (u -> v: przepływ/pojemność):")
        for u in 1:length(adj)
            for e in adj[u]
                if e.capacity > 0 
                    @printf("%d -> %d: %d/%d\n", u-1, e.to-1, e.flow, e.capacity)
                end
            end
        end
    end
    
    elapsed = end_time - start_time
    @warn "Statystyki wykonania" Czas_s=elapsed Liczba_sciezek=p_count
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end