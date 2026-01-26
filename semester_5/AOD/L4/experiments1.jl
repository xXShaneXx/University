using Plots
using Statistics
using Printf

include("ex1.jl")

function run_comprehensive_benchmarks()
    k_values = 1:16
    
    # Kontenery na uśrednione wyniki
    results_flow = Float64[]
    results_paths = Float64[]
    results_times = Float64[]

    println("="^60)
    @printf("%-5s | %-15s | %-12s | %-10s\n", "k", "Śr. Przepływ", "Śr. Ścieżki", "Czas (s)")
    println("-"^60)

    for k in k_values
        # Liczba powtórzeń (zmniejszana dla dużych k, by skrypt skończył się dzisiaj)
        num_reps = k <= 10 ? 10 : (k <= 13 ? 3 : 1)
        
        local_flows = Int[]
        local_paths = Int[]
        local_times = Float64[]

        for _ in 1:num_reps
            start_t = time()
            
            # 1. Budowa grafu 
            adj = build_hypercube(k)
            
            # 2. Uruchomienie algorytmu 
            f, p = edmonds_karp(adj, 1, 2^k)
            
            push!(local_times, time() - start_t)
            push!(local_flows, f)
            push!(local_paths, p)
        end

        # Wyciąganie średnich
        avg_f = mean(local_flows)
        avg_p = mean(local_paths)
        avg_t = mean(local_times)

        push!(results_flow, avg_f)
        push!(results_paths, avg_p)
        push!(results_times, avg_t)

        @printf("%-5d | %-15.1f | %-12.1f | %-10.4f\n", k, avg_f, avg_p, avg_t)
    end
    println("="^60)

    # --- Generowanie wykresów ---
    
    # Wykres 1: Maksymalny przepływ (Skala logarytmiczna z uwagi na wykładniczy wzrost)
    p1 = plot(k_values, results_flow, title="Średni Max Flow", 
              xlabel="k", ylabel="Przepływ", yaxis=:log10, 
              marker=:circle, label="Przepływ")

    # Wykres 2: Liczba ścieżek powiększających
    p2 = plot(k_values, results_paths, title="Ścieżki powiększające", 
              xlabel="k", ylabel="Liczba ścieżek", 
              marker=:square, color=:red, label="Ścieżki")

    # Wykres 3: Czas wykonania (Skala logarytmiczna)
    p3 = plot(k_values, results_times, title="Czas wykonania", 
              xlabel="k", ylabel="Sekundy", yaxis=:log10, 
              marker=:utriangle, color=:green, label="Czas")

    # Składanie w jeden raport
    final_plot = plot(p1, p2, p3, layout=(3,1), size=(800, 1000), margin=5Plots.mm)
    
    # Eksport wyników
    savefig(final_plot, "results_k1_k16.png")
    println("Eksperyment zakończony. Wykresy zapisano jako 'results_k1_k16.png'.")
end

# Uruchomienie badania
run_comprehensive_benchmarks()