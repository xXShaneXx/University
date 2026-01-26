using JuMP
using GLPK
using DataStructures
using Statistics
using Printf
using Plots

# Zakładamy, że ex1.jl i ex2.jl są w tym samym katalogu
include("ex1.jl")
include("ex2.jl")

# --- Rozwiązywanie problemu przepływu przez LP (GLPK) ---
# Zmiana: dodano argument return_flows
function solve_flow_jump(adj, s, t; return_flows=false)
    model = Model(GLPK.Optimizer)
    # set_silent(model) # Odkomentuj, aby ukryć logi solvera
    
    n = length(adj)
    # Przygotowanie listy krawędzi (u, v, capacity, original_index)
    edges = []
    for u in 1:n
        for (idx, e) in enumerate(adj[u])
            if e.capacity > 0 
                push!(edges, (u, e.to, e.capacity, idx))
            end
        end
    end
    
    @variable(model, 0 <= x[1:length(edges)])
    
    for i in 1:length(edges)
        @constraint(model, x[i] <= edges[i][3])
    end
    
    for node in 1:n
        if node == s || node == t continue end
        in_indices = [i for i in 1:length(edges) if edges[i][2] == node]
        out_indices = [i for i in 1:length(edges) if edges[i][1] == node]
        
        if !isempty(in_indices) || !isempty(out_indices)
            @constraint(model, sum(x[i] for i in out_indices) == sum(x[i] for i in in_indices))
        end
    end
    
    source_out_indices = [i for i in 1:length(edges) if edges[i][1] == s]
    @objective(model, Max, sum(x[i] for i in source_out_indices))
    
    optimize!(model)
    obj_val = objective_value(model)

    if return_flows
        # Zbieramy przepływy do słownika (u, v) -> flow
        lp_flows = Dict{Tuple{Int,Int}, Float64}()
        x_vals = value.(x)
        for i in 1:length(edges)
            u, v = edges[i][1], edges[i][2]
            if x_vals[i] > 1e-6 # Ignorujemy bliskie zeru
                lp_flows[(u, v)] = x_vals[i]
            end
        end
        return obj_val, lp_flows
    else
        return obj_val
    end
end

# --- Nowa funkcja weryfikująca zgodność wektorów ---
function verify_vectors(adj, lp_flows, k)
    println("  -> Weryfikacja wektorów przepływu dla k=$k...")
    
    # Pobranie przepływów z Edmondsa-Karpa (są w strukturze adj)
    ek_flows = Dict{Tuple{Int,Int}, Int}()
    for u in 1:length(adj)
        for e in adj[u]
            if e.capacity > 0 && e.flow > 0
                ek_flows[(u, e.to)] = e.flow
            end
        end
    end
    
    # Porównanie
    all_keys = union(keys(ek_flows), keys(lp_flows))
    identical = true
    max_diff = 0.0
    
    for (u, v) in all_keys
        val_ek = get(ek_flows, (u, v), 0)
        val_lp = get(lp_flows, (u, v), 0.0)
        diff = abs(val_ek - val_lp)
        if diff > 1e-5
            identical = false
            max_diff = max(max_diff, diff)
        end
    end
    
    if identical
        println("     [OK] Wektory przepływu są identyczne (EK == LP).")
    else
        println("     [INFO] Wektory są RÓŻNE (max różnica: $(round(max_diff, digits=2))).")
    end
end

# --- Rozwiązywanie problemu skojarzenia przez LP (GLPK) ---
function solve_matching_jump(adj, n1, n2)
    model = Model(GLPK.Optimizer)
    set_silent(model)
    
    edges = []
    for u in 1:n1
        for v in adj[u]
            push!(edges, (u, v))
        end
    end
    
    @variable(model, x[1:length(edges)], Bin)
    
    for u in 1:n1
        u_edges = [i for i in 1:length(edges) if edges[i][1] == u]
        if !isempty(u_edges) 
            @constraint(model, sum(x[i] for i in u_edges) <= 1) 
        end
    end
    
    for v in 1:n2
        v_edges = [i for i in 1:length(edges) if edges[i][2] == v]
        if !isempty(v_edges) 
            @constraint(model, sum(x[i] for i in v_edges) <= 1) 
        end
    end
    
    @objective(model, Max, sum(x))
    optimize!(model)
    return objective_value(model)
end

# --- Procedura Benchmarkingu i Generowania Wykresów ---

function run_comparison_and_plot()
    println("=== ROZPOCZYNANIE BENCHMARKU LP vs ALGORYTMY ===")
    
    k_range_flow = 3:9
    k_range_match = 3:9
    
    times_ek = Float64[]
    times_lp_flow = Float64[]
    
    times_hk = Float64[]
    times_lp_match = Float64[]
    
    println("\n--- 1. Porównanie: Edmonds-Karp vs GLPK ---")
    for k in k_range_flow
        println("Przetwarzanie k=$k...")
        adj = build_hypercube(k)
        s, t = 1, 2^k
        
        # Pomiar czasu Edmonds-Karp
        t_ek = @elapsed val_ek, _ = edmonds_karp(adj, s, t)
        push!(times_ek, t_ek)
        
        # Pomiar czasu LP (GLPK) i weryfikacja dla małych k
        if k <= 4
            # Dla małych k pobieramy też wektory przepływu
            t_lp = @elapsed begin
                val_lp, lp_flows = solve_flow_jump(adj, s, t, return_flows=true)
            end
            # Weryfikacja wektorów (Zadanie 3)
            verify_vectors(adj, lp_flows, k)
        else
            t_lp = @elapsed val_lp = solve_flow_jump(adj, s, t)
        end
        
        push!(times_lp_flow, t_lp)
        
        # Weryfikacja wartości Max Flow
        if val_ek != val_lp
            @warn "Niezgodność wyników dla k=$k! EK=$val_ek, LP=$val_lp"
        else
            println("  Max Flow zgodny: $val_ek")
        end
    end
    
    println("\n--- 2. Porównanie: Hopcroft-Karp vs GLPK ---")
    for k in k_range_match
        println("Przetwarzanie k=$k...")
        i_deg = max(1, k ÷ 2)
        adj = generate_bipartite_graph(k, i_deg)
        size_v = 2^k
        
        t_hk = @elapsed val_hk, _ = hopcroft_karp(adj, size_v, size_v)
        push!(times_hk, t_hk)
        
        t_lp = @elapsed val_lp = solve_matching_jump(adj, size_v, size_v)
        push!(times_lp_match, t_lp)
        
        if val_hk != val_lp
            @warn "Niezgodność wyników dla k=$k! HK=$val_hk, LP=$val_lp"
        end
    end
    
    println("\nGenerowanie wykresów...")
    
    p1 = plot(k_range_flow, [times_ek times_lp_flow],
        label=["Edmonds-Karp" "GLPK (LP)"],
        title="Czas: Max Flow",
        xlabel="Rozmiar (k)", ylabel="Czas (s)",
        yaxis=:log10, marker=:auto, lw=2
    )
     
    p2 = plot(k_range_match, [times_hk times_lp_match],
        label=["Hopcroft-Karp" "GLPK (LP)"],
        title="Czas: Max Matching",
        xlabel="Rozmiar (k)", ylabel="Czas (s)",
        yaxis=:log10, marker=:auto, lw=2
    )
    
    final_plot = plot(p1, p2, layout=(2,1), size=(800, 800), margin=5Plots.mm)
    savefig(final_plot, "comparison_lp_vs_algo.png")
    
    println("Zakończono. Wykres zapisano jako 'comparison_lp_vs_algo.png'.")
end

if abspath(PROGRAM_FILE) == @__FILE__
    run_comparison_and_plot()
end