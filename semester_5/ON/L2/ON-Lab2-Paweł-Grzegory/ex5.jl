#Paweł Grzegory 282211

using Printf

function population_map(p0::T, r::T, n::Int) where T<:AbstractFloat
    p = Vector{T}(undef, n + 1)
    p[1] = p0
    for i in 1:n
        p[i+1] = p[i] + r * p[i] * (1 - p[i])
    end
    return p
end

function run_experiments()
    println("--- Eksperyment 1: Porównanie standardowej iteracji z iteracją z obciętym wynikiem (Float32) ---")
    p0_32 = Float32(0.01)
    r_32 = Float32(3.0)
    n_iterations = 40

    # 1. Standardowe 40 iteracji
    results_normal = population_map(p0_32, r_32, n_iterations)

    # 2. Iteracje z modyfikacją po 10-tym kroku
    results_modified_part1 = population_map(p0_32, r_32, 10)
    
    # Obcięcie wyniku z 10. iteracji do 3 miejsc po przecinku
    p10_truncated = round(results_modified_part1[end], digits=3)
    
    # Kontynuacja obliczeń od obciętej wartości
    remaining_iterations = n_iterations - 10
    p_cont = Vector{Float32}(undef, remaining_iterations + 1)
    p_cont[1] = p10_truncated
    for i in 1:remaining_iterations
        p_cont[i+1] = p_cont[i] + r_32 * p_cont[i] * (1 - p_cont[i])
    end
    
    # Połączenie wyników
    results_modified = vcat(results_modified_part1[1:11], p_cont[2:end])


    println("\nPorównanie wyników dla Float32:")
    @printf "%-5s %-20s %-20s %-20s\n" "Iter" "Normalna" "Zmodyfikowana" "Różnica"
    println("-"^65)
    for i in 1:n_iterations+1
        if i <= length(results_normal) && i <= length(results_modified)
            diff = abs(results_normal[i] - results_modified[i])
            @printf "%-5d %-20.15f %-20.15f %-20.15e\n" i-1 results_normal[i] results_modified[i] diff
        end
    end
    println("\nPo 10 iteracjach wynik $(results_modified_part1[end]) został zastąpiony przez $p10_truncated.")

    println("\n\n--- Eksperyment 2: Porównanie arytmetyki Float32 i Float64 ---")
    p0_64 = Float64(0.01)
    r_64 = Float64(3.0)

    results_float64 = population_map(p0_64, r_64, n_iterations)

    println("\nPorównanie wyników dla Float32 i Float64:")
    @printf "%-5s %-25s %-25s %-25s\n" "Iter" "Float32" "Float64" "Różnica"
    println("-"^85)
    for i in 1:n_iterations+1
        diff = abs(Float64(results_normal[i]) - results_float64[i])
        @printf "%-5d %-25.15f %-25.15f %-25.15e\n" i-1 results_normal[i] results_float64[i] diff
    end
end

run_experiments()
