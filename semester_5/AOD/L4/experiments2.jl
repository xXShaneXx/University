using Plots
using Statistics
include("ex2.jl")

function run_experiments()
    k_vals = 3:10
    repeats = 5
    
    # 1. Wykres wielkości skojarzenia f(i) dla każdego k
    println("Rozpoczynanie eksperymentów dla wielkości skojarzenia...")
    plt_matching = plot(title="Wielkość skojarzenia vs Stopień (i)", xlabel="i", ylabel="Rozmiar skojarzenia")
    
    for k in k_vals
        i_range = 1:k
        avg_matches = Float64[]
        for i in i_range
            m_sizes = [hopcroft_karp(generate_bipartite_graph(k, i), 2^k, 2^k)[1] for _ in 1:repeats]
            push!(avg_matches, mean(m_sizes))
        end
        plot!(plt_matching, i_range, avg_matches, label="k=$k", marker=:o)
    end
    savefig(plt_matching, "matching_size_vs_i.png")

    # 2. Wykres czasu f(k) dla każdego i (wybrane wartości i)
    println("Rozpoczynanie eksperymentów dla czasu działania...")
    plt_time = plot(title="Czas działania vs Rozmiar (k)", xlabel="k", ylabel="Czas [s]", yaxis=:log)
    
    # Przykładowo dla i=1, i=k/2, i=k
    for i_type in [:min, :mid, :max]
        avg_times = Float64[]
        for k in k_vals
            i = i_type == :min ? 1 : (i_type == :mid ? max(1, k ÷ 2) : k)
            times = Float64[]
            for _ in 1:repeats
                t = @elapsed hopcroft_karp(generate_bipartite_graph(k, i), 2^k, 2^k)
                push!(times, t)
            end
            push!(avg_times, mean(times))
        end
        label = i_type == :min ? "i=1" : (i_type == :mid ? "i=k/2" : "i=k")
        plot!(plt_time, k_vals, avg_times, label=label, marker=:square)
    end
    savefig(plt_time, "execution_time_vs_k.png")
    
    println("Eksperymenty zakończone. Wykresy zapisano w folderze roboczym.")
end

run_experiments()