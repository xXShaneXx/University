using Plots
using Statistics
using Printf
using DataStructures

# Importujemy struktury i funkcję budowy grafu z ex1.jl
include("ex1.jl")

"""
Implementacja algorytmu Shortest Augmenting Path (SAP)
Złożoność: O(n^2 * m) dzięki etykietom odległości i gap heuristic.
"""
function shortest_augmenting_path(adj, s, t)
    n = length(adj)
    max_flow = 0
    path_count = 0
    
    dist = fill(n, n)
    count_dist = fill(0, n + 1)
    
    # Inicjalizacja etykiet odległości (wsteczny BFS z t)
    queue = Deque{Int}()
    push!(queue, t)
    dist[t] = 0
    while !isempty(queue)
        u = popfirst!(queue)
        count_dist[dist[u] + 1] += 1
        for e in adj[u]
            rev_e = adj[e.to][e.rev]
            if dist[e.to] == n && rev_e.capacity > 0
                dist[e.to] = dist[u] + 1
                push!(queue, e.to)
            end
        end
    end
    if dist[s] == n count_dist[n + 1] = n end # Jeśli brak ścieżki na starcie

    ptr = fill(1, n)
    curr = s
    parent_edge = fill(0, n) # Przechowuje indeks krawędzi do rodzica
    parent_node = fill(0, n)
    
    while dist[s] < n
        if curr == t
            # Znaleziono ścieżkę - szukaj bottleneck
            push_flow = typemax(Int)
            tmp = t
            while tmp != s
                p = parent_node[tmp]
                idx = parent_edge[tmp]
                push_flow = min(push_flow, adj[p][idx].capacity - adj[p][idx].flow)
                tmp = p
            end
            
            # Aktualizacja
            tmp = t
            while tmp != s
                p = parent_node[tmp]
                idx = parent_edge[tmp]
                rev_idx = adj[p][idx].rev
                adj[p][idx].flow += push_flow
                adj[tmp][rev_idx].flow -= push_flow
                tmp = p
            end
            
            max_flow += push_flow
            path_count += 1
            curr = s # Powrót do źródła
            continue
        end
        
        found = false
        for i in ptr[curr]:length(adj[curr])
            e = adj[curr][i]
            if e.capacity - e.flow > 0 && dist[curr] == dist[e.to] + 1
                found = true
                ptr[curr] = i
                parent_node[e.to] = curr
                parent_edge[e.to] = i
                curr = e.to
                break
            end
        end
        
        if !found
            count_dist[dist[curr] + 1] -= 1
            if count_dist[dist[curr] + 1] == 0 break end # Gap heuristic
            
            min_d = n
            for e in adj[curr]
                if e.capacity - e.flow > 0
                    min_d = min(min_d, dist[e.to] + 1)
                end
            end
            dist[curr] = min_d
            count_dist[dist[curr] + 1] += 1
            ptr[curr] = 1
            if curr != s
                curr = parent_node[curr]
            end
        end
    end
    
    return max_flow, path_count
end

function run_comparison_benchmark()
    k_values = 1:14
    ek_times, sap_times = Float64[], Float64[]
    ek_paths, sap_paths = Float64[], Float64[]
    ek_flows, sap_flows = Float64[], Float64[] # Nowe kontenery na przepływ

    println("="^85)
    @printf("%-3s | %-10s | %-10s | %-10s | %-10s | %-12s\n", "k", "T_EK (s)", "T_SAP (s)", "P_EK", "P_SAP", "Max Flow")
    println("-"^85)

    for k in k_values
        # Test Edmonds-Karp [cite: 1]
        adj_ek = build_hypercube(k)
        t_ek = @elapsed f_ek, p_ek = edmonds_karp(adj_ek, 1, 2^k)
        
        # Test SAP (Zadanie 4)
        adj_sap = build_hypercube(k)
        t_sap = @elapsed f_sap, p_sap = shortest_augmenting_path(adj_sap, 1, 2^k)
        
        push!(ek_times, t_ek); push!(sap_times, t_sap)
        push!(ek_paths, p_ek); push!(sap_paths, p_sap)
        push!(ek_flows, f_ek); push!(sap_flows, f_sap)

        @printf("%-3d | %-10.4f | %-10.4f | %-10d | %-10d | %-12d\n", 
                k, t_ek, t_sap, p_ek, p_sap, f_ek)
    end

    # --- Generowanie Wykresów ---
    
    # 1. Wykres Czasu (skala logarytmiczna)
    p1 = plot(k_values, [ek_times sap_times], 
              label=["Edmonds-Karp" "SAP"], 
              title="Czas wykonania", 
              xlabel="k", ylabel="Sekundy", yaxis=:log10, marker=:auto)

    # 2. Wykres Liczby Ścieżek
    p2 = plot(k_values, [ek_paths sap_paths], 
              label=["Edmonds-Karp" "SAP"], 
              title="Liczba ścieżek powiększających", 
              xlabel="k", ylabel="Sztuki", marker=:auto)

    # 3. Wykres Maksymalnego Przepływu
    p3 = plot(k_values, [ek_flows sap_flows], 
              label=["Edmonds-Karp" "SAP"], 
              title="Maksymalny przepływ", 
              xlabel="k", ylabel="Wartość przepływu", yaxis=:log10, 
              linestyle=[:solid :dash], marker=:auto)

    # Składanie wykresów w jeden raport
    combined = plot(p1, p2, p3, layout=(3,1), size=(800, 1100), margin=5Plots.mm)
    savefig(combined, "full_comparison_ek_vs_sap.png")
    println("\nPełne zestawienie (w tym przepływ) zapisano do 'full_comparison_ek_vs_sap.png'.")
end

run_comparison_benchmark()

